#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>
using namespace std;

#include <QPixmap>
#include <QGuiApplication>
#include <QScreen>

#define CLAMP_RENDER_DIMENSIONS false
#define MAX_SIZE_SCALE_FACTOR   0.7
#define DEFAULT -1

void Image::free() {
    if (this->isEmpty()) { return; }

    if (this->_window) {
        delete this->_window;
        this->_window = nullptr;
    }

    if (this->_data != NULL)
        stbi_image_free(this->_data);
    this->_data = NULL;
    this->_pxls.clear();

    this->_isGrayscale = false;
    this->_maxL = -1;
    this->_minL = -1;

    fprintf(stderr, "Destroying Image\n");
}

void Image::buildPixelsMatrix() {
    _pxls = vector<vector<Pixel>>(width(), vector<Pixel>(height()));
    for (int x = 0; x < width(); x++) {
        for (int y = 0; y < height(); y++) {
            _pxls[x][y] = Pixel(data(x, y));
        }
    }
}

void Image::setData(unsigned char *newData) {
    if (this->_data != NULL)
        stbi_image_free(this->_data);
    this->_data = newData;
    buildPixelsMatrix();
}

void Image::load(const char *filename) {
    if (!this->isEmpty()) {
        this->free();
    }

    _filename = string(filename);
    _isGrayscale = channels() == 1;

    setData(stbi_load(filename, &w, &h, &c, 0));
}

void Image::createWindow(QString windowTitle, bool destroyOnClose) {
    _window = new ImageWindow(windowTitle);

    connect(_window, &ImageWindow::onClose, this, &Image::onClose);
    if (destroyOnClose)
        connect(_window, &ImageWindow::onClose, this, &Image::free);

    render();
}

Image::Image(const char *filename) {
    load(filename);
}

Image::Image(const char *filename, QString windowTitle, bool destroyOnClose):
    Image(filename) {
    createWindow(windowTitle, destroyOnClose);
}

Image::Image(Image *fromImg) {
    if (fromImg->isEmpty()) { return; }

    _filename = string(fromImg->filename());

    w = fromImg->width();
    h = fromImg->height();
    c = fromImg->channels();

    _isGrayscale = fromImg->isGrayscale();

    size_t size = w*h*c * sizeof(unsigned char);
    unsigned char *newData = (unsigned char*)malloc(size);
    memcpy(newData, fromImg->data(), size);

   setData(newData);
}

Image::Image(Image *fromImg, QString windowTitle, bool destroyOnClose):
    Image(fromImg) {
    createWindow(windowTitle, destroyOnClose);
}

Image::~Image() {
    this->free();
}

bool Image::save(const char *filename, int quality) {
    if (isEmpty()) return false;

    string f(filename);
    string ext = f.substr(f.find_last_of('.')+1);
    for_each(ext.begin(), ext.end(), [](char &c) { c = ::tolower(c); });

    if (ext == "jpg")
        return stbi_write_jpg(filename, width(), height(), channels(), data(), quality) != 0;

    throw runtime_error("Image::save - Unsupported file extension");

    return false;
}

void Image::copy(Image *img) {
    if (isEmpty()) return;

    if (w == img->w && h == img->h && c == img->c) {
        memcpy(_data, img->_data, w*h*c * sizeof(unsigned char));
    } else {
        size_t size = img->w*img->h*img->c * sizeof(unsigned char);
        unsigned char *newData = (unsigned char*)malloc(size);
        memcpy(newData, img->_data, size);

        this->w = img->w;
        this->h = img->h;
        this->c = img->c;
        this->setData(newData);
    }

    _isGrayscale = false;
    _minL = -1;
    _maxL = -1;
}

void Image::render() {
    if (isEmpty()) return;
    if (!window()) return;

    int w = width(), h = height(), c = channels();

    // Create QImage
    QImage qimg(data(), w, h, w*c, QImage::Format_RGB888);

    // Define dimensions and resize if needed
#if CLAMP_RENDER_DIMENSIONS
    QRect screenRect = QGuiApplication::primaryScreen()->geometry();
    int MAX_W = screenRect.width() * MAX_SIZE_SCALE_FACTOR;
    int MAX_H = screenRect.height() * MAX_SIZE_SCALE_FACTOR;
    int pixmapWidth = DEFAULT, pixmapHeight = DEFAULT;

    if (w > MAX_W && h > MAX_H) {
        if (w/MAX_W > h/MAX_H) {
            pixmapWidth = MAX_W;
        } else {
            pixmapHeight = MAX_H;
        }
    } else if (w > MAX_W) {
        pixmapWidth = MAX_W;
    } else if (h > MAX_H) {
        pixmapHeight = MAX_H;
    }
#endif

    QPixmap pixmap;
#if CLAMP_RENDER_DIMENSIONS
    if (pixmapWidth == DEFAULT && pixmapHeight == DEFAULT) {
        pixmap = QPixmap::fromImage(qimg);
    } else if (pixmapWidth != DEFAULT) {
        pixmap = QPixmap::fromImage(qimg).scaledToWidth(pixmapWidth);
    } else if (pixmapHeight != DEFAULT) {
        pixmap = QPixmap::fromImage(qimg).scaledToHeight(pixmapHeight);
    }
#else
    pixmap = QPixmap::fromImage(qimg);
#endif

    window()->setFixedSize(QSize(pixmap.width(), pixmap.height()));
    window()->setPixmap(pixmap);
    window()->show();
}

// > Image processing

void Image::flipHorizontally() {
    if (isEmpty()) return;

    int pixelSize = channels();
    unsigned char tmp[pixelSize];
    for (int x = 0; x < width()/2; x++) {
        for (int y = 0; y < height(); y++) {
            unsigned char *leftPx = data(x, y);
            unsigned char *rightPx = data(width()-1 - x, y);

            memcpy(tmp, leftPx, pixelSize*sizeof(unsigned char));
            memcpy(leftPx, rightPx, pixelSize*sizeof(unsigned char));
            memcpy(rightPx, tmp, pixelSize*sizeof(unsigned char));
        }
    }
}

void Image::flipVertically() {
    if (isEmpty()) return;

    int rowSize = channels() * width();
    unsigned char tmp[rowSize];
    for (int y = 0; y < height()/2; y++) {
        unsigned char *topRow = row(y);
        unsigned char *bottomRow = row(height()-1 - y);

        memcpy(tmp, topRow, rowSize*sizeof(unsigned char));
        memcpy(topRow, bottomRow, rowSize*sizeof(unsigned char));
        memcpy(bottomRow, tmp, rowSize*sizeof(unsigned char));
    }
}

void Image::toGrayScale() {
    if (isEmpty()) { return; }
    if (_isGrayscale) { return; }

    _maxL = -1;
    _minL = 256;

    for (int x = 0; x < width(); x++) {
        for (int y = 0; y < height(); y++) {
            Pixel p = pixel(x, y);
            int L = p.luminance();
            p.rgb(L, L, L);

            if (L > _maxL) { _maxL = L; }
            if (L < _minL) { _minL = L; }
        }
    }

    _isGrayscale = true;
}

void Image::quantize(int n) {
    if (isEmpty()) { return; }
    if (!_isGrayscale) { toGrayScale(); }

    int range = _maxL - _minL + 1;
    int minL = _minL;

    if (n < range) {
        _maxL = -1;
        _minL = 999;

        float binSize = (float)range / n;
        for (int x = 0; x < width(); x++) {
            for (int y = 0; y < height(); y++) {
                Pixel p = pixel(x, y);
                int L = p.luminance();
                // Calculate `index` of the L value
                int iL = L - minL;
                // Calculate `index` of the bin to which the L value should be mapped to
                int iBin = iL / binSize;
                // Calculate midpoint of the bin interval
                L = int((float)minL - 0.5 + (float)iBin * binSize + binSize / 2.0);
                p.rgb(L, L, L);

                if (L > _maxL) { _maxL = L; }
                if (L < _minL) { _minL = L; }
            }
        }
    }
}

Histogram Image::grayscaleHistogram() {
    Histogram histogram;

    if (isEmpty()) return histogram;

    for (int x = 0; x < width(); x++) {
        for (int y = 0; y < height(); y++) {
            Pixel p = pixel(x, y);
            int L = p.luminance();
            histogram[L]++;
        }
    }

    return histogram;
}

void Image::adjustBrightness(int brightness) {
    if (isEmpty()) return;

    for (int x = 0; x < width(); x++) {
        for (int y = 0; y < height(); y++) {
            Pixel p = pixel(x, y);
            p += brightness;
        }
    }
}

void Image::adjustContrast(double contrast) {
    if (isEmpty()) return;

    for (int x = 0; x < width(); x++) {
        for (int y = 0; y < height(); y++) {
            Pixel p = pixel(x, y);
            p *= contrast;
        }
    }
}

void Image::toNegative() {
    if (isEmpty()) return;

    for (int x = 0; x < width(); x++) {
        for (int y = 0; y < height(); y++) {
            Pixel p = pixel(x, y);
            p.red(255 - p.red());
            p.blue(255 - p.blue());
            p.green(255 - p.green());
        }
    }
}

Histogram Image::cumulativeGrayscaleHistogram() {
    Histogram histogram = grayscaleHistogram();
    double a = 255.0 / (double)(width() * height());
    return histogram.accumulateAndScale(a);
}

void Image::equalizeHistogram() {
    if (isEmpty()) return;

    // Compute cumulative histogram
    Histogram hist_cum = cumulativeGrayscaleHistogram();

    // Use renormalized cumulative histogram of the Luminance channel for equalization
    for (int x = 0; x < width(); x++) {
        for (int y = 0; y < height(); y++) {
            Pixel p = pixel(x, y);
            for (int c = 0; c < channels(); c++) {
                p.channel(Channel(c), hist_cum[p.channel(Channel(c))]);
            }
        }
    }
}

void Image::matchHistogramOf(Image *target) {
    if (isEmpty()) return;

    // First, convert both source and target images to grayscale
    this->toGrayScale();
    target->toGrayScale();

    // Compute cumulative histogram for source and target images
    Histogram Hsrc = this->cumulativeGrayscaleHistogram();
    Histogram Htgt = target->cumulativeGrayscaleHistogram();

    // Compute histogram matching function
    // For each source shade find the target shade with most similar cumulative value
    Histogram HM;
    for (int shade = 0; shade < 256; shade++) {
        HM[shade] = indexOfNearest(Htgt.asVector(), Hsrc[shade]);
    }

    // Apply histogram matching function to source image
    for (int x = 0; x < width(); x++) {
        for (int y = 0; y < height(); y++) {
            Pixel p = pixel(x, y);
            int newL = HM[p.luminance()];
            p.rgb(newL, newL, newL);
        }
    }
}

void Image::rotate(RotationDirection direction) {
    if (isEmpty()) return;

    // in : [H][W]
    // out: [W][H]
    int inW = width(), inH = height();
    int outW = inH, outH = inW;
    unsigned char *out = (unsigned char*)malloc(outH*outW*c * sizeof(unsigned char));

    for (int y = 0; y < inH; y++) {
        for (int x = 0; x < inW; x++) {
            int inX = x, inY = y;
            int outX, outY;

            if (direction == ClockWise) {
                // in[y][x] -> out[h-1-y][x]
                outX = inH-1 - inY;
                outY = inX;
            } else { // CounterClockwise
                // in[y][x] -> out[w-1-x][y]
                outX = inY;
                outY = inW-1 - inX;
            }

            Pixel outPixel(&out[dataIndex(outW, outH, c, outX, outY)]);
            outPixel << this->pixel(x, y);
        }
    }

    this->w = outW;
    this->h = outH;
    this->setData(out);
}

void Image::zoomOut(int sx, int sy) {
    if (isEmpty()) return;

    int inW = width(), inH = height();
    // outW = ceil(inW/sx), outH = ceil(inH/sy);
    int outW = (inW+sx-1)/sx, outH = (inH+sy-1)/sy;
    unsigned char *out = (unsigned char*)malloc(outH*outW*c * sizeof(unsigned char));

    for (int x = 0; x < inW; x += sx) {
        for (int y = 0; y < inH; y += sy) {
            // out[y/sy][x/sx] = mean(in[y][x] : in[y+sy-1][x+sx-1])
            vector<int> rgbSum(c, 0);
            int n = 0;
            for (int iX = x; iX < x+sx && iX < inW; iX++) {
                for (int iY = y; iY < y+sy && iY < inH; iY++) {
                    Pixel inP = pixel(x, y);
                    for (int iC = 0; iC < c; iC++)
                        rgbSum[iC] += inP.channel(Channel(iC));
                    n++;
                }
            }

            int outX = x/sx, outY = y/sy;
            Pixel outPixel(&out[dataIndex(outW, outH, c, outX, outY)]);
            for (int iC = 0; iC < c; iC++) {
                int v = (n > 0) ? rgbSum[iC]/n : rgbSum[iC];
                outPixel.channel(Channel(iC), v);
            }
        }
    }

    this->w = outW;
    this->h = outH;
    this->setData(out);
}

void Image::zoomIn() {
    if (isEmpty()) return;

    int inW = width(), inH = height();
    int outW = 2*inW-1, outH = 2*inH-1;
    unsigned char *out = (unsigned char*)malloc(outH*outW*c * sizeof(unsigned char));

    // Compute even rows of output image
    for (int y = 0; y < inH; y++) {
        for (int x = 0; x < outW; x++) {
            Pixel outPixel(&out[dataIndex(outW, outH, c, x, y*2)]);
            if (x % 2 == 0) {
                // out[y*2][x] = in[y][x/2]
                outPixel << this->pixel(x/2, y);
            } else {
                // out[y*2][x] = (in[y][x/2] + in[y][(x+1)/2]) / 2
                Pixel in0 = this->pixel(x/2, y);
                Pixel in1 = this->pixel((x+1)/2, y);
                for (int iC = 0; iC < c; iC++) {
                    int v = (in0.channel(Channel(iC)) + in1.channel(Channel(iC))) / 2;
                    outPixel.channel(Channel(iC), v);
                }
            }
        }
    }

    // Compute the odd rows of the output image
    for (int y = 1; y < outH; y += 2) {
        for (int x = 0; x < outW; x++) {
            // out[y][x] = (in[y/2][x/2] + in[(y+1)/2][x/2]) / 2
            Pixel outPixel(&out[dataIndex(outW, outH, c, x, y)]);
            Pixel in0 = this->pixel(x/2, y/2);
            Pixel in1 = this->pixel(x/2, (y+1)/2);
            for (int iC = 0; iC < c; iC++) {
                int v = (in0.channel(Channel(iC)) + in1.channel(Channel(iC))) / 2;
                outPixel.channel(Channel(iC), v);
            }
        }
    }

    this->w = outW;
    this->h = outH;
    this->setData(out);
}

void Image::convolve(Kernel kernel, bool grayBackground) {
    if (isEmpty()) return;

    unsigned char *out = (unsigned char*)calloc(h*w*c, sizeof(unsigned char));

    for (int x = 1; x < width()-1; x++) {
        for (int y = 1; y < height()-1; y++) {
            double acc = grayBackground ? 127 : 0;
            for (int dx = 0; dx < kernel.width(); dx++) {
                for (int dy = 0; dy < kernel.height(); dy++) {
                    // acc += K[dx][dy] * I[x+(1-dx)][y+(1-dy)]
                    Pixel p = pixel(x + (1-dx), y + (1-dy));
                    acc += kernel.at(dx, dy) * p.luminance();
                }
            }

            Pixel outPixel(&out[dataIndex(w, h, c, x, y)]);
            outPixel.rgb(acc, acc, acc);
        }
    }

    this->setData(out);
}

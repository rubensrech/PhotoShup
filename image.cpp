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

#define CLAMP_RENDER_DIMENSIONS true
#define MAX_SIZE_SCALE_FACTOR   0.7
#define DEFAULT -1

void Image::free() {
    if (this->isEmpty()) { return; }

    if (this->_window) {
        delete this->_window;
        this->_window = nullptr;
    }

    stbi_image_free(this->_data);
    this->_data = NULL;
    this->_pxls.clear();

    this->_isGrayscale = false;
    this->_maxL = -1;
    this->_minL = -1;

    fprintf(stderr, "Destroying Image\n");
}

void Image::buildPixelsMatrix() {
    _pxls.resize(width(), vector<Pixel>(height()));
    for (int x = 0; x < width(); x++) {
        for (int y = 0; y < height(); y++) {
            _pxls[x][y] = Pixel(data(x, y));
        }
    }
}

void Image::load(const char *filename) {
    if (!this->isEmpty()) {
        this->free();
    }

    _filename = string(filename);
    _data = stbi_load(filename, &w, &h, &c, 0);

    _isGrayscale = channels() == 1;

    buildPixelsMatrix();
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

    size_t size = w*h*c * sizeof(unsigned char);
    _data = (unsigned char*)malloc(size);
    memcpy(_data, fromImg->data(), size);

    _isGrayscale = fromImg->isGrayscale();

    buildPixelsMatrix();
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
    if (w != img->w || h != img->h || c != img->c) return;

    memcpy(_data, img->data(), w*h*c * sizeof(unsigned char));

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
            p.red(p.red() + brightness);
            p.blue(p.blue() + brightness);
            p.green(p.green() + brightness);
        }
    }
}

void Image::adjustContrast(double contrast) {
    if (isEmpty()) return;

    for (int x = 0; x < width(); x++) {
        for (int y = 0; y < height(); y++) {
            Pixel p = pixel(x, y);
            p.red((double)p.red() * contrast);
            p.blue((double)p.blue() * contrast);
            p.green((double)p.green() * contrast);
        }
    }
}

void Image::toNegative() {
    if (isEmpty()) return;

    for (int x = 0; x < width(); x++) {
        for (int y = 0; y < height(); y++) {
            Pixel p = pixel(x, y);
            p.red(255- p.red());
            p.blue(255 - p.blue());
            p.green(255 - p.green());
        }
    }
}

Histogram Image::cumulativeGrayscaleHistogram() {
    Histogram histogram = grayscaleHistogram();
    float a = 255.0 / (width() * height());
    return histogram.accumulateAndNormalize(a);
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
                p.channel(c, hist_cum[p.channel(c)]);
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

void Image::rotateClockwise() {
    if (isEmpty()) return;

    // in : [H][W]
    // out: [W][H]
    int inW = this->w, inH = this->h;
    int outW = inH, outH = inW;
    unsigned char *out = (unsigned char*)malloc(outH*outW*c * sizeof(unsigned char));

    for (int y = 0; y < inH; y++) {
        for (int x = 0; x < inW; x++) {
            // in[y][x] -> out[h-1-y][x]
            int inX = x, inY = y;
            int outX = inH-1 - inY, outY = inX;
            Pixel outPixel(&out[dataIndex(outW, outH, c, outX, outY)]);
            outPixel.copy(this->pixel(x, y));
        }
    }

    this->w = outW;
    this->h = outH;
    stbi_image_free(this->_data);
    this->_data = out;

    buildPixelsMatrix();
}

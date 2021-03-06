#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <QPixmap>

#include <iostream>
using namespace std;

void Image::free() {
    if (this->isEmpty()) { return; }

    stbi_image_free(this->data);
    this->data = NULL;
    this->pxls.clear();

    this->isGrayscale = false;
    this->maxL = -1;
    this->minL = -1;
}

void Image::load(const char *filename) {
    if (!this->isEmpty()) {
        this->free();
    }

    this->filename = string(filename);
    this->data = stbi_load(filename, &this->w, &this->h, &this->c, 0);

    this->isGrayscale = c == 1;

    this->pxls.resize(this->w, vector<Pixel>(this->h));
    for (int x = 0; x < this->width(); x++) {
        for (int y = 0; y < this->height(); y++) {
            this->pxls[x][y] = Pixel(this->getData(x, y));
        }
    }
}

bool Image::save(const char *filename, int quality) {
    string f(filename);
    string ext = f.substr(f.find_last_of('.')+1);
    for_each(ext.begin(), ext.end(), [](char &c) { c = ::tolower(c); });

    if (ext == "jpg")
        return stbi_write_jpg(filename, this->width(), this->height(), this->channels(), this->data, quality) != 0;

    throw runtime_error("Image::save - Unsupported file extension");

    return false;
}

void Image::copy(Image *img) {
    memcpy(data, img->data, width()*height()*channels()*sizeof(unsigned char));
    isGrayscale = false;
}

Image::Image(const char *filename) {
    this->load(filename);
}

Image::~Image() {
    this->free();
}

// > Image processing

void Image::flipHorizontally() {
    int pixelSize = this->channels();
    unsigned char tmp[pixelSize];
    for (int x = 0; x < this->width()/2; x++) {
        for (int y = 0; y < this->height(); y++) {
            unsigned char *leftPx = this->getData(x, y);
            unsigned char *rightPx = this->getData(this->width()-1 - x, y);

            memcpy(tmp, leftPx, pixelSize*sizeof(unsigned char));
            memcpy(leftPx, rightPx, pixelSize*sizeof(unsigned char));
            memcpy(rightPx, tmp, pixelSize*sizeof(unsigned char));
        }
    }
}

void Image::flipVertically() {
    int rowSize = this->channels() * this->width();
    unsigned char tmp[rowSize];
    for (int y = 0; y < this->height()/2; y++) {
        unsigned char *topRow = this->getRow(y);
        unsigned char *bottomRow = this->getRow(this->height()-1 - y);

        memcpy(tmp, topRow, rowSize*sizeof(unsigned char));
        memcpy(topRow, bottomRow, rowSize*sizeof(unsigned char));
        memcpy(bottomRow, tmp, rowSize*sizeof(unsigned char));
    }
}

void Image::toGrayScale() {
    if (this->isEmpty()) { return; }
    if (this->isGrayscale) { return; }

    this->maxL = -1;
    this->minL = 256;

    for (int x = 0; x < this->width(); x++) {
        for (int y = 0; y < this->height(); y++) {
            Pixel p = this->pixel(x, y);
            int L = 0.299*p.red() + 0.587*p.green() + 0.114*p.blue();
            p.rgb(L, L, L);

            if (L > this->maxL) { this->maxL = L; }
            if (L < this->minL) { this->minL = L; }
        }
    }

    this->isGrayscale = true;
}

bool Image::quantize(int n) {
    if (isEmpty()) { return false; }
    if (!isGrayscale) {
        this->toGrayScale();
    }

    int range = maxL - minL + 1;
    int minL = this->minL;

    if (n < range) {
        this->maxL = -1;
        this->minL = 999;

        float binSize = (float)range / n;
        for (int x = 0; x < width(); x++) {
            for (int y = 0; y < height(); y++) {
                Pixel p = pixel(x, y);
                int L = p.red();
                // Calculate `index` of the L value
                int iL = L - minL;
                // Calculate `index` of the bin to which the L value should be mapped to
                int iBin = iL / binSize;
                // Calculate midpoint of the bin interval
                L = int((float)minL - 0.5 + (float)iBin * binSize + binSize / 2.0);
                p.rgb(L, L, L);

                if (L > this->maxL) { this->maxL = L; }
                if (L < this->minL) { this->minL = L; }
            }
        }
    }

    return true;
}

int* Image::grayscaleHistogram() {
    if (isEmpty()) { return nullptr; }
    if (!isGrayscale) {
        this->toGrayScale();
    }

    int *histogram = new int[256];

    for (int i = 0; i < 256; i++) {
        histogram[i] = 0;
    }

    for (int x = 0; x < width(); x++) {
        for (int y = 0; y < height(); y++) {
            histogram[pixel(x,y).red()]++;
        }
    }

    return histogram;
}



#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <QPixmap>

Image::Image(const char *filename) {
    if (!this->isEmpty()) {
        stbi_image_free(this->data);
        this->pxls.clear();
    }

    this->data = stbi_load(filename, &this->w, &this->h, &this->c, 0);
    this->pxls.resize(this->w, vector<Pixel>(this->h));

    for (int x = 0; x < this->width(); x++) {
        for (int y = 0; y < this->height(); y++) {
            this->pxls[x][y] = Pixel(this->getData(x, y));
        }
    }
}

Image::~Image() {
    if (this->isEmpty()) { return; }

    stbi_image_free(this->data);
    this->data = NULL;
}

void Image::flipHorizontal() {
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

void Image::flipVertical() {
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

    for (int x = 0; x < this->width(); x++) {
        for (int y = 0; y < this->height(); y++) {
            Pixel p(this->pixel(x, y));
            int L = 0.299*p.red() + 0.587*p.green() + 0.114*p.blue();
            p.rgb(L, L, L);
        }
    }
}




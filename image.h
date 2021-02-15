#ifndef IMAGE_H
#define IMAGE_H

#include "pixel.h"

#include <vector>
using namespace std;

class Image {

// > Properties

private:
    unsigned char *data = NULL;

    vector<vector<Pixel>> pxls;

    int w;
    int h;
    int c;

    bool isGrayscale = false;
    int maxL = -1;
    int minL = -1;


// > Methods

private:
    unsigned char *getData(int x, int y, int c = 0) {
        if (!this->isEmpty())
            return &this->data[this->channels()*(y*this->width() + x) + c];
        return NULL;
    };

    unsigned char *getRow(int y) { return this->getData(0, y); }

public:
    // > Constructors / Destructors
    Image(const char *filename);
    ~Image();

    // > Getters
    bool isEmpty() { return this->data == NULL; }
    int width() { return this->w; }
    int height() { return this->h; }
    int channels() { return this->c; }
    Pixel pixel(int x, int y) { return this->pxls[x][y]; }

    void flipHorizontal();
    void flipVertical();
    void toGrayScale();
    bool quantize(int n);

};

#endif // IMAGE_H

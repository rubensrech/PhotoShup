#ifndef IMAGE_H
#define IMAGE_H

#include "pixel.h"

#include <vector>
#include <string>
using namespace std;

class Image {

// > Properties

private:
    string filename;

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
    void free();

public:
    // > Constructors / Destructors
    Image(const char *filename);
    ~Image();

    void load(const char *filename);
    bool save(const char *filename, int quality = 90);
    void copy(Image *img);

    string extension() { return filename.substr(filename.find_last_of(".")+1); }

    bool isEmpty() { return this->data == NULL; }

    int scaledWidth(int h) { return width()*h/height(); };
    int scaledHeight(int w) { return height()*w/width(); };

    // > Getters
    unsigned char *getData() { return data; }
    unsigned char *getData(int x, int y, int c = 0) { return (!isEmpty()) ? &data[this->c*(y*w + x) + c] : NULL; }
    unsigned char *getRow(int y) { return this->getData(0, y); }
    int width() { return this->w; }
    int height() { return this->h; }
    int channels() { return this->c; }
    Pixel pixel(int x, int y) { return this->pxls[x][y]; }

    // Image processing
    void flipHorizontally();
    void flipVertically();
    void toGrayScale();
    bool quantize(int n);

};

#endif // IMAGE_H

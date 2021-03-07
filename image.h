#ifndef IMAGE_H
#define IMAGE_H

#include "histogram.h"
#include "imagewindow.h"
#include "pixel.h"

#include <vector>
#include <string>
using namespace std;

class Image {

// > Properties

private:
    string _filename;

    unsigned char *_data = NULL;
    vector<vector<Pixel>> _pxls;

    int w;
    int h;
    int c;

    bool _isGrayscale = false;
    int _maxL = -1;
    int _minL = -1;

    ImageWindow *_window = nullptr;

// > Methods

private:
    void free();
    void load(const char *filename);

public:
    // > Constructors / Destructors
    Image(const char *filename);
    Image(const char *filename, QString windowTitle);
    ~Image();


    bool save(const char *filename, int quality = 90);
    void copy(Image *img);

    void render();

    // > Getters

    string filename() { return _filename; }
    ImageWindow *window() { return _window; }

    bool isEmpty() { return _data == NULL; }

    unsigned char *data() { return _data; }

    Pixel pixel(int x, int y) { return _pxls[x][y]; }

    int width() { return w; }
    int height() { return h; }
    int channels() { return c; }

    string extension() { return filename().substr(filename().find_last_of(".")+1); }

    unsigned char *data(int x, int y, int c = 0) { return (!isEmpty()) ? &data()[this->c*(y*w + x) + c] : NULL; }
    unsigned char *row(int y) { return data(0, y); }

    int scaledWidth(int h) { return width()*h/height(); };
    int scaledHeight(int w) { return height()*w/width(); };

    // > Image processing

    void flipHorizontally();
    void flipVertically();
    void toGrayScale();
    void quantize(int n);
    Histogram grayscaleHistogram();
    void adjustBrightness(int brightness);
    void adjustContrast(double contrast);
    void toNegative();

};

#endif // IMAGE_H

#ifndef PIXEL_H
#define PIXEL_H

#include <algorithm>
using namespace std;

enum Channel {
    Red = 0,
    Green = 1,
    Blue = 2,
};

class Pixel {

// > Properties

private:
    unsigned char *r;
    unsigned char *g;
    unsigned char *b;

// > Methods

private:
    int clamp(int val, int lower = 0, int upper = 255) { return max(lower, min(val, upper)); }

public:
    // > Constructors
    Pixel();
    Pixel(unsigned char *data);

    // > Getters
    int red()   { return *r; }
    int green() { return *g; }
    int blue()  { return *b; }
    int channel(int c) {
        switch (c) {
        case 0: return red();
        case 1: return blue();
        case 2: return green();
        default: return -1;
        }
    }

    int luminance() { return 0.299*red() + 0.587*green() + 0.114*blue(); }

    // > Setters
    void red(int r)   { *this->r = clamp(r); }
    void green(int g) { *this->g = clamp(g); }
    void blue(int b)  { *this->b = clamp(b); }
    void channel(int c, int v) {
        switch (c) {
        case 0: red(v); break;
        case 1: blue(v); break;
        case 2: green(v); break;
        }
    }

    void rgb(int r, int g, int b) {
        this->red(r);
        this->green(g);
        this->blue(b);
    }

};

#endif // PIXEL_H

#ifndef PIXEL_H
#define PIXEL_H

#include "util.h"

enum Channel {
    Red = 0,
    Green = 1,
    Blue = 2,
};

class Pixel {

// > Properties

private:
    unsigned char *r = NULL;
    unsigned char *g = NULL;
    unsigned char *b = NULL;

// > Methods

private:
    void setData(unsigned char *data) {
        this->r = data;
        this->g = &data[1];
        this->b = &data[2];
    }

public:
    // > Constructors
    Pixel() {};
    Pixel(unsigned char *data) { setData(data); }



    // > Getters
    int red()   { return r != NULL ? *r : -1; }
    int green() { return g != NULL ? *g : -1; }
    int blue()  { return b != NULL ? *b : -1; }
    int channel(int c) {
        switch (c) {
        case 0: return red();
        case 1: return blue();
        case 2: return green();
        default: return -1;
        }
    };

    /*
     * Some extra precision was added to the weights,
     * because both values 0.299 and 0.587 cannot be
     * exactly represented in binary floating-point
     * format
     */
    int luminance() { return 0.29900001*red() + 0.58700001*green() + 0.114*blue(); }

    // > Setters
    void red(int r)   { if (this->r != NULL) *this->r = clamp(r, 0, 255); }
    void green(int g) { if (this->g != NULL) *this->g = clamp(g, 0, 255); }
    void blue(int b)  { if (this->b != NULL) *this->b = clamp(b, 0, 255); }
    void channel(int c, int v) {
        switch (c) {
        case 0: red(v); break;
        case 1: blue(v); break;
        case 2: green(v); break;
        }
    }

    void rgb(int r, int g, int b) {
        red(r);
        green(g);
        blue(b);
    }

    void copy(Pixel p) { rgb(p.red(), p.green(), p.blue()); }

    // > Operators
    void operator+=(int v) { rgb(red()+v, green()+v, blue()+v); }


};

#endif // PIXEL_H

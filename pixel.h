#ifndef PIXEL_H
#define PIXEL_H

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

public:
    // > Constructors
    Pixel();
    Pixel(unsigned char *data);

    // > Getters
    int red()   { return *this->r; }
    int green() { return *this->g; }
    int blue()  { return *this->b; }

    // > Setters
    void red(int r)   { *this->r = r; }
    void green(int g) { *this->g = g; }
    void blue(int b)  { *this->b = b; }
    void rgb(int r, int g, int b) {
        this->red(r);
        this->green(g);
        this->blue(b);
    }

};

#endif // PIXEL_H

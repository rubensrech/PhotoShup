#include "pixel.h"

Pixel::Pixel() {}

Pixel::Pixel(unsigned char *data) {
    this->r = data;
    this->g = &data[1];
    this->b = &data[2];
}

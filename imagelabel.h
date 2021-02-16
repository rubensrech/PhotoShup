#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include "image.h"

#include <QLabel>

class ImageLabel : public QLabel
{
private:
    Image *img;

public:
    ImageLabel(Image *img);
    ImageLabel(Image *img, QWidget *parent);
    ImageLabel(Image *img, QString windowTitle);

    void render();
};

#endif // IMAGEWINDOW_H

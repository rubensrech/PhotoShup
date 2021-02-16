#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include "image.h"

#include <QLabel>

class ImageLabel : public QLabel {

private:
    Image *img;
    int pixmapWidth;
    int pixmapHeight;

public:
    ImageLabel(QWidget *parent, Image *img = nullptr);
    ImageLabel(QString windowTitle, Image *img = nullptr);

    void setImage(Image *img);
    void render();
};

#endif // IMAGEWINDOW_H

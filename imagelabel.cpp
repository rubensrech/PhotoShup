#include "imagelabel.h"

#include <QBitmap>

#define MAX_HEIGHT 600

ImageLabel::ImageLabel(QWidget *parent, Image *img)
        : QLabel(parent) {
    this->setImage(img);
}

ImageLabel::ImageLabel(QString windowTitle, Image *img)
        : QLabel() {
    this->setWindowTitle(windowTitle);
    this->setImage(img);
}

void ImageLabel::setImage(Image *img) {
    if (!img) {
        this->hide();
        return;
    }

    this->img = img;

    if (img->height() > MAX_HEIGHT) {
        this->pixmapHeight = MAX_HEIGHT;
        this->pixmapWidth = img->scaledWidth(MAX_HEIGHT);
    } else {
        this->pixmapHeight = img->height();
        this->pixmapWidth = img->width();
    }

    this->render();
    this->setFixedSize(QSize(this->pixmapWidth, this->pixmapHeight));
    this->show();
}

void ImageLabel::render() {
    if (!this->img) return;

    int w = img->width(), h = img->height(), c = img->channels();
    QImage qimg(img->getData(), w, h, w*c, QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(qimg).scaledToHeight(this->pixmapHeight);

    this->setPixmap(pixmap);
}

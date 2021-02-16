#include "imagelabel.h"

#include <QBitmap>

#define IMAGE_HEIGHT 600

ImageLabel::ImageLabel(Image *img): QLabel() {
    this->img = img;
    this->render();
}

ImageLabel::ImageLabel(Image *img, QWidget *parent):
        ImageLabel(img) {
    this->setParent(parent);
}

ImageLabel::ImageLabel(Image *img, QString windowTitle):
        ImageLabel(img) {
    this->show();
    this->setWindowTitle(windowTitle);
    this->setFixedSize(QSize(img->scaledWidth(IMAGE_HEIGHT), IMAGE_HEIGHT));
}

void ImageLabel::render() {
    if (!this->img) return;

    int w = img->width(), h = img->height(), c = img->channels();
    QImage qimg(img->getData(), w, h, w*c, QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(qimg).scaledToHeight(IMAGE_HEIGHT);

    this->setPixmap(pixmap);
}

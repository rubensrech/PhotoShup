#include "imagelabel.h"

#include <QBitmap>
#include <QGuiApplication>
#include <QScreen>

#define DEFAULT -1
#define MAX_SCALE_FACTOR 0.7

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

    this->render();
    this->show();
}

void ImageLabel::render() {
    if (!this->img) return;
    int w = img->width(), h = img->height(), c = img->channels();

    // Create QImage
    QImage qimg(img->getData(), w, h, w*c, QImage::Format_RGB888);

    // Define dimensions and resize if needed
    QRect screenRect = QGuiApplication::primaryScreen()->geometry();
    int MAX_W = screenRect.width() * MAX_SCALE_FACTOR;
    int MAX_H = screenRect.height() * MAX_SCALE_FACTOR;
    int pixmapWidth = DEFAULT, pixmapHeight = DEFAULT;
    QPixmap pixmap;

    if (w > MAX_W && h > MAX_H) {
        if (w/MAX_W > h/MAX_H) {
            pixmapWidth = MAX_W;
        } else {
            pixmapHeight = MAX_H;
        }
    } else if (w > MAX_W) {
        pixmapWidth = MAX_W;
    } else if (h > MAX_H) {
        pixmapHeight = MAX_H;
    }

    if (pixmapWidth == DEFAULT && pixmapHeight == DEFAULT) {
        pixmap = QPixmap::fromImage(qimg);
    } else if (pixmapWidth != DEFAULT) {
        pixmap = QPixmap::fromImage(qimg).scaledToWidth(pixmapWidth);
    } else if (pixmapHeight != DEFAULT) {
        pixmap = QPixmap::fromImage(qimg).scaledToHeight(pixmapHeight);
    }

    this->setFixedSize(QSize(pixmap.width(), pixmap.height())); 
    this->setPixmap(pixmap);
}

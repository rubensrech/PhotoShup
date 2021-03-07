#include "imagewindow.h"

#include <QBitmap>

ImageWindow::ImageWindow(QString windowTitle) : QLabel() {
    this->setWindowTitle(windowTitle);
}

ImageWindow::~ImageWindow() {}

void ImageWindow::closeEvent(QCloseEvent *event) {
    emit onClose();
}

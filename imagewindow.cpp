#include "imagewindow.h"

#include <QBitmap>

ImageWindow::ImageWindow(QString windowTitle) : QLabel() {
    this->setWindowTitle(windowTitle);
}

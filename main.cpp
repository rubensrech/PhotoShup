#include "mainwindow.h"
#include "image.h"

#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    Image image("/Users/rubensrechjunior/Downloads/IMG_2657.jpg");
    printf("Empty: %d\n", image.isEmpty());
    int x = 200, y = 200;
    Pixel p = image.pixel(x, y);
    printf("(%d,%d) = rgb(%d,%d,%d)\n", x, y, p.red(), p.green(), p.blue());

    image.toGrayScale();
    // image.flipVertical();
    // image.flipHorizontal();

    // Show image
    QLabel label;
    QImage qimg(image.width(), image.height(), QImage::Format_RGB888);
    for (int x = 0; x < image.width(); x++) {
        for (int y = 0; y < image.height(); y++) {
            Pixel p = image.pixel(x, y);
            qimg.setPixelColor(x, y, QColor(p.red(), p.green(), p.blue()));
        }
    }

    label.setPixmap(QPixmap::fromImage(qimg).scaled(600, 600, Qt::KeepAspectRatio));
    label.show();


    return a.exec();
}

#include "mainwindow.h"
#include "image.h"

#include <QApplication>
#include <QLabel>

#include <string>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainWindow w(nullptr);
//    MainWindow w("/Users/rubensrechjunior/Downloads/IMG_2657.jpg");
//    MainWindow w("/Users/rubensrechjunior/Downloads/test_images/Underwater_53k.jpg");
    w.show();

    // w.handleOpenClicked();

    return a.exec();
}

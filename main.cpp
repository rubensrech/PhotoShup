#include "mainwindow.h"
#include "image.h"

#include <QApplication>
#include <QLabel>

#include <string>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainWindow w("/Users/rubensrechjunior/Downloads/IMG_2657.jpg");
    w.show();

    return a.exec();
}

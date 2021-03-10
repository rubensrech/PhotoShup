#include "mainwindow.h"

#include <QApplication>
#include <QLabel>

#include <string>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainWindow w("/Users/rubensrechjunior/Downloads/FPI/IMG_2710-small.jpg");
    w.show();

    return a.exec();
}

#include "mainwindow.h"
#include "image.h"

#include <QApplication>
#include <QLabel>

#include <string>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}

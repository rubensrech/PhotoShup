﻿#include "imagewindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QPushButton>

#include <iostream>
using namespace std;

#define X_MARGIN 50
#define Y_MARGIN 80

MainWindow::MainWindow(const char *filename):
    QMainWindow(), ui(new Ui::MainWindow) {

    ui->setupUi(this);
    this->setWindowTitle("Image editor");

    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(centralWidget);

    controls = new ControlsWrapper(centralWidget);
    connect(controls, &ControlsWrapper::imgFileSelected, this, &MainWindow::openImgFile);
    connect(controls, &ControlsWrapper::saveClicked, this, &MainWindow::saveImg);
    connect(controls, &ControlsWrapper::copyClicked, this, &MainWindow::copyOriginalImg);
    connect(controls, &ControlsWrapper::hflipClicked, this, &MainWindow::hflipImg);
    connect(controls, &ControlsWrapper::vflipClicked, this, &MainWindow::vflipImg);
    connect(controls, &ControlsWrapper::grayscaleClicked, this, &MainWindow::grayscaleImg);
    connect(controls, &ControlsWrapper::quantizeClicked, this, &MainWindow::quantizeImg);
    connect(controls, &ControlsWrapper::histogramClicked, this, &MainWindow::showImgHistogram);
    layout->addWidget(controls);

    this->setCentralWidget(centralWidget);
    this->adjustSize();

    if (filename) {
        origImg = new Image(filename, "Original image");
        img = new Image(filename, "Edited image");
    } else {
        controls->setDisabled(true);
    }
}

MainWindow::~MainWindow() {
    delete ui;

    if (img) delete img;
    if (origImg) delete origImg;
}

void MainWindow::openImgFile(const char *filename) {
    if (origImg) delete origImg;
    if (img) delete img;

    origImg = new Image(filename, "Original image");
    img = new Image(filename, "Edited image");

    controls->setDisabled(false);
    adjustSize();

    // Adjust windows position
    int origImgX = X_MARGIN, origImgY = Y_MARGIN;
    int origImgW = origImg->window()->width(), origImgH = origImg->window()->height();

    int imgX = 2*X_MARGIN + origImgW, imgY = Y_MARGIN;
    int imgW = img->window()->width(), imgH = img->window()->height();

    int controlsX = imgX + imgW + X_MARGIN, controlsY = Y_MARGIN;
    int controlsW = this->width(), controlsH = this->height();

    origImg->window()->setGeometry(origImgX, origImgY, origImgW, origImgH);
    img->window()->setGeometry(imgX, imgY, imgW, imgH);
    this->setGeometry(controlsX, controlsY, controlsW, controlsH);
}

void MainWindow::saveImg() {
    QString origFilename = QString::fromStdString(img->filename());
    QString filename = QFileDialog::getSaveFileName(this, "Save image", origFilename);
    if (!filename.isEmpty() && !filename.isNull()) {
        img->save(filename.toStdString().c_str());
    }
}

void MainWindow::copyOriginalImg() {
    img->copy(origImg);
    img->render();
}

void MainWindow::hflipImg() {
    img->flipHorizontally();
    img->render();
}

void MainWindow::vflipImg() {
    img->flipVertically();
    img->render();
}

void MainWindow::grayscaleImg() {
    img->toGrayScale();
    img->render();
}

void MainWindow::quantizeImg(int n) {
    img->quantize(n);
    img->render();
}

void MainWindow::showImgHistogram() {
    int *histogram = img->grayscaleHistogram();
    img->render();

    for (int i = 0; i < 256; i++) {
        cout << i << ":" << histogram[i] << endl;
    }

    delete histogram;

    cout << "Free:" << histogram << endl;
}


void MainWindow::closeEvent(__attribute__((unused)) QCloseEvent *bar) {
    // Quit application when MainWindow (controls) is closed
    QApplication::quit();
}

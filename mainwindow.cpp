#include "imagelabel.h"
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
    connect(controls, &ControlsWrapper::hflipClicked, this, &MainWindow::hflipImg);
    connect(controls, &ControlsWrapper::vflipClicked, this, &MainWindow::vflipImg);
    connect(controls, &ControlsWrapper::grayscaleClicked, this, &MainWindow::grayscaleImg);
    connect(controls, &ControlsWrapper::quantizeClicked, this, &MainWindow::quantizeImg);
    connect(controls, &ControlsWrapper::copyClicked, this, &MainWindow::copyOriginalImg);
    connect(controls, &ControlsWrapper::saveClicked, this, &MainWindow::saveImg);
    layout->addWidget(controls);

    this->setCentralWidget(centralWidget);
    this->adjustSize();

    origImgLabel = new ImageLabel("Original image");
    imgLabel = new ImageLabel("Edited image");

    if (filename) {
        origImg = new Image(filename);
        img = new Image(filename);
        origImgLabel->setImage(origImg);
        imgLabel->setImage(img);
    } else {
        controls->setDisabled(true);
    }
}

MainWindow::~MainWindow() {
    delete ui;

    if (origImgLabel) delete origImgLabel;
    if (imgLabel) delete imgLabel;

    if (img) delete img;
    if (origImg) delete origImg;
}

void MainWindow::openImgFile(const char *filename) {
    if (origImg) delete origImg;
    if (img) delete img;

    origImg = new Image(filename);
    img = new Image(filename);

    origImgLabel->setImage(origImg);
    imgLabel->setImage(img);

    controls->setDisabled(false);
    adjustSize();

    // Adjust windows position
    int origImgX = X_MARGIN, origImgY = Y_MARGIN;
    int origImgW = origImgLabel->width(), origImgH = origImgLabel->height();

    int imgX = 2*X_MARGIN + origImgW, imgY = Y_MARGIN;
    int imgW = imgLabel->width(), imgH = imgLabel->height();

    int controlsX = imgX + imgW + X_MARGIN, controlsY = Y_MARGIN;
    int controlsW = this->width(), controlsH = this->height();

    origImgLabel->setGeometry(origImgX, origImgY, origImgW, origImgH);
    imgLabel->setGeometry(imgX, imgY, imgW, imgH);
    this->setGeometry(controlsX, controlsY, controlsW, controlsH);
}

void MainWindow::hflipImg() {
    img->flipHorizontally();
    imgLabel->render();
}

void MainWindow::vflipImg() {
    img->flipVertically();
    imgLabel->render();
}

void MainWindow::grayscaleImg() {
    img->toGrayScale();
    imgLabel->render();
}

void MainWindow::quantizeImg(int n) {
    img->quantize(n);
    imgLabel->render();
}

void MainWindow::copyOriginalImg() {
    img->copy(origImg);
    imgLabel->render();
}

void MainWindow::saveImg() {
    QString origFilename = QString::fromStdString(img->getFilename());
    QString filename = QFileDialog::getSaveFileName(this, "Save image", origFilename);
    if (!filename.isEmpty() && !filename.isNull()) {
        img->save(filename.toStdString().c_str());
    }
}

void MainWindow::closeEvent(__attribute__((unused)) QCloseEvent *bar) {
    // Quit application when MainWindow (controls) is closed
    QApplication::quit();
}

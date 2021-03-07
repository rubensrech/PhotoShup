#include "imagewindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QPushButton>

#include <algorithm>
#include <iostream>
using namespace std;

#define X_MARGIN 30
#define Y_MARGIN 50

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
    connect(controls, &ControlsWrapper::brightnessClicked, this, &MainWindow::adjustBrightness);
    connect(controls, &ControlsWrapper::contrastClicked, this, &MainWindow::adjustContrast);
    connect(controls, &ControlsWrapper::negativeClicked, this, &MainWindow::negativeImg);
    connect(controls, &ControlsWrapper::equalizeHistogramClicked, this, &MainWindow::equalizeHistogram);
    layout->addWidget(controls);

    this->setCentralWidget(centralWidget);
    this->adjustSize();

    if (filename) {
        openImgFile(filename);
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

    connect(origImg, &Image::onClose, this, &MainWindow::origImgWasClosed);
    connect(img, &Image::onClose, this, &MainWindow::editImgWasClosed);

    controls->setDisabled(false);
    adjustSize();

    // Adjust windows position
    QRect screen = QGuiApplication::primaryScreen()->geometry();

    int origImgX = X_MARGIN, origImgY = Y_MARGIN;
    int origImgW = origImg->window()->width();

    int imgX = min(origImgX + origImgW + X_MARGIN/2, screen.width() - X_MARGIN);
    int imgY = Y_MARGIN;
    int imgW = img->window()->width();

    int controlsX = min(imgX + imgW + X_MARGIN/2, screen.width() - X_MARGIN);
    int controlsY = Y_MARGIN;

    origImg->window()->move(origImgX, origImgY);
    img->window()->move(imgX, imgY);
    this->move(controlsX, controlsY);
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
    img->toGrayScale();
    img->grayscaleHistogram().show();
    img->render();
}

void MainWindow::adjustBrightness(int brightness) {
    img->adjustBrightness(brightness);
    img->render();
}

void MainWindow::adjustContrast(double contrast) {
    img->adjustContrast(contrast);
    img->render();
}

void MainWindow::negativeImg() {
    img->toNegative();
    img->render();
}

void MainWindow::equalizeHistogram() {
    QRect screen = QGuiApplication::primaryScreen()->geometry();

    // Display copy of the image before histogram equalization
    // Memory is deallocated when window is closed
    ImageWindow *imgBeforeWindow = (new Image(img, "Image: Before equalization", true))->window();

    // Set the position of the image before histogram equalization
    ImageWindow *origImgWindow = origImg->window();
    int imgBeforeX = origImgWindow->x();
    int imgBeforeY = origImgWindow->y();
    imgBeforeWindow->move(imgBeforeX, imgBeforeY);

    bool isGrayscale = img->isGrayscale();
    if (isGrayscale) {
        // Display histogram of the image before equalization
        QChartView *hBefore = img->grayscaleHistogram().show("Histogram: Before equalization");

        // Set the position of the histogram (before equalization)
        int hBeforeX = imgBeforeWindow->x();
        int hBeforeY = min(imgBeforeWindow->height() + imgBeforeWindow->y(), screen.width() - Y_MARGIN);
        hBefore->move(hBeforeX, hBeforeY);
    }

    img->equalizeHistogram();
    img->render();

    if (isGrayscale) {
        // Display histogram of the image after equalization
        QChartView *hAfter = img->grayscaleHistogram().show("Histogram: After equalization");

        // Set the position of the histogram (after equalization)
        ImageWindow *imgAfterWindow = img->window();
        int hAfterX = imgAfterWindow->x();
        int hAfterY = min(imgAfterWindow->height() + imgAfterWindow->y(), screen.width() - Y_MARGIN);
        hAfter->move(hAfterX, hAfterY);
    }
}


// > Close events

void MainWindow::origImgWasClosed() {
    fprintf(stderr, "MainWindow: Original Image was closed\n");
}

void MainWindow::editImgWasClosed() {
    fprintf(stderr, "MainWindow: Edit Image was closed\n");
     QApplication::quit();
}

void MainWindow::closeEvent(__attribute__((unused)) QCloseEvent *event) {
    // Quit application when MainWindow (controls) is closed
    QApplication::quit();
}

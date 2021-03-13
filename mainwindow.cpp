#include "imagewindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QPushButton>

#include <algorithm>
#include <iostream>
#include <chrono>
using namespace std;

#define X_MARGIN 30
#define Y_MARGIN 50

MainWindow::MainWindow(const char *filename):
    QMainWindow(), ui(new Ui::MainWindow) {

    ui->setupUi(this);
    this->setWindowTitle("Image editor");

    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(centralWidget);
    layout->setMargin(0);
    layout->setSpacing(0);

    controls = new ControlsWrapper(centralWidget);
    connect(controls, &ControlsWrapper::openClicked, this, &MainWindow::openImgFile);
    connect(controls, &ControlsWrapper::saveClicked, this, &MainWindow::saveImg);
    connect(controls, &ControlsWrapper::copyClicked, this, &MainWindow::copyOriginalImg);
    connect(controls, &ControlsWrapper::hflipClicked, this, &MainWindow::hflipImg);
    connect(controls, &ControlsWrapper::vflipClicked, this, &MainWindow::vflipImg);
    connect(controls, &ControlsWrapper::grayscaleClicked, this, &MainWindow::grayscaleImg);
    connect(controls, &ControlsWrapper::quantizeClicked, this, &MainWindow::quantizeImg);
    connect(controls, &ControlsWrapper::showHistogramClicked, this, &MainWindow::showImgHistogram);
    connect(controls, &ControlsWrapper::brightnessClicked, this, &MainWindow::adjustBrightness);
    connect(controls, &ControlsWrapper::contrastClicked, this, &MainWindow::adjustContrast);
    connect(controls, &ControlsWrapper::negativeClicked, this, &MainWindow::negativeImg);
    connect(controls, &ControlsWrapper::equalizeHistogramClicked, this, &MainWindow::equalizeHistogram);
    connect(controls, &ControlsWrapper::matchHistogramClicked, this, &MainWindow::matchHistogram);
    connect(controls, &ControlsWrapper::rotateClicked, this, &MainWindow::rotate);
    connect(controls, &ControlsWrapper::zoomOutClicked, this, &MainWindow::zoomOut);
    connect(controls, &ControlsWrapper::zoomInClicked, this, &MainWindow::zoomIn);
    connect(controls, &ControlsWrapper::convolveClicked, this, &MainWindow::convolve);
    layout->addWidget(controls);

    this->setCentralWidget(centralWidget);
    this->adjustSize();

    if (filename) {
        if (!openImgFile(filename)) {
            this->hide();
            QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
        }
    } else {
        controls->setDisabled(true);
    }
}

MainWindow::~MainWindow() {
    delete ui;

    if (img) delete img;
    if (origImg) delete origImg;
}

bool MainWindow::openImgFile(const char *filename) {
    QFileInfo fileInfo(filename);
    if (!fileInfo.exists() || !fileInfo.isFile()) {
        int ret = QMessageBox::critical(nullptr, "File not found",
                                    "Could not open the image file because it was not found.");
        return !ret;
    }

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

    int imgW = img->window()->width();
    int imgX = min(origImgX + origImgW + X_MARGIN/2, screen.width() - imgW);
    int imgY = Y_MARGIN;

    int controlsW = this->width();
    int controlsX = min(imgX + imgW + X_MARGIN/2, screen.width() - controlsW);
    int controlsY = Y_MARGIN;

    origImg->window()->move(origImgX, origImgY);
    img->window()->move(imgX, imgY);
    this->move(controlsX, controlsY);

    // Bring controls window to front
    this->raise();
    this->activateWindow();

    return true;
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

    // Reset img window position
    QRect screen = QGuiApplication::primaryScreen()->geometry();
    ImageWindow *origImgWindow = origImg->window();
    ImageWindow *imgWindow = img->window();
    int imgX = min(origImgWindow->x() + origImgWindow->width() + X_MARGIN/2, screen.width() - imgWindow->width());
    int imgY = Y_MARGIN;
    imgWindow->move(imgX, imgY);
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
    int imgBeforeX = origImgWindow->x() + 10;
    int imgBeforeY = origImgWindow->y() + 10;
    imgBeforeWindow->move(imgBeforeX, imgBeforeY);

    bool isGrayscale = img->isGrayscale();
    if (isGrayscale) {
        // Display histogram of the image before equalization
        QChartView *hBefore = img->grayscaleHistogram().show("Histogram: Before equalization");

        // Set the position of the histogram (before equalization)
        int hBeforeH = hBefore->height();
        int hBeforeX = imgBeforeWindow->x();
        int hBeforeY = min(imgBeforeWindow->height() + imgBeforeWindow->y(), screen.height() - hBeforeH);
        hBefore->move(hBeforeX, hBeforeY);
    }

    img->equalizeHistogram();
    img->render();

    if (isGrayscale) {
        // Display histogram of the image after equalization
        QChartView *hAfter = img->grayscaleHistogram().show("Histogram: After equalization");

        // Set the position of the histogram (after equalization)
        ImageWindow *imgAfterWindow = img->window();
        int hAfterH = hAfter->height();
        int hAfterX = imgAfterWindow->x();
        int hAfterY = min(imgAfterWindow->height() + imgAfterWindow->y(), screen.height() - hAfterH);
        hAfter->move(hAfterX, hAfterY);
    }
}

void MainWindow::matchHistogram(const char *targetFilename) {
    QRect screen = QGuiApplication::primaryScreen()->geometry();

    // Load and show target image
    Image *targetImg = new Image(targetFilename, "Image: Histogram Matching target", true);

    // Set target image position
    ImageWindow *targetImgWindow = targetImg->window();
    ImageWindow *origImgWindow = origImg->window();
    int targetImgX = origImgWindow->x() + 10, targetImgY = origImgWindow->y() + 10;
    targetImgWindow->move(targetImgX, targetImgY);

    // Perform the histogram matching operation
    img->matchHistogramOf(targetImg);

    // Display target image histogram
    QChartView *hTarget = targetImg->grayscaleHistogram().show("Histogram: Histogram Matching target");
    // Set the position of target image histogram
    int hTargetH = hTarget->height();
    int hTargetX = targetImgWindow->x();
    int hTargetY = min(targetImgWindow->y() + targetImgWindow->height(), screen.height() - hTargetH);
    hTarget->move(hTargetX, hTargetY);

    // Display source image histogram
    QChartView *hSrc = img->grayscaleHistogram().show("Histogram: Histogram Matching source");
    // Set the position of source image histogram
    ImageWindow *srcImgWindow = img->window();
    int hSrcH = hSrc->height();
    int hSrcX = srcImgWindow->x();
    int hSrcY = min(srcImgWindow->y() + srcImgWindow->height(), screen.height() - hSrcH);
    hSrc->move(hSrcX, hSrcY);

    // Re-render both images once they were converted to grayscale
    img->render();
    targetImg->render();
}

void MainWindow::rotate(RotationDirection direction) {
    img->rotate(direction);
    img->render();

    // Move controls window position
    QRect screen = QGuiApplication::primaryScreen()->geometry();
    ImageWindow *imgWindow = img->window();
    int controlsW = this->width();
    int controlsX = min(imgWindow->x() + imgWindow->width() + X_MARGIN/2, screen.width() - controlsW);
    int controlsY = imgWindow->y();
    this->move(controlsX, controlsY);
}

void MainWindow::zoomOut(int sx, int sy) {
    auto started = chrono::high_resolution_clock::now();
    img->zoomOut(sx, sy);
    auto done = chrono::high_resolution_clock::now();
    cout << "Zoom out: " << chrono::duration_cast<chrono::milliseconds>(done-started).count() << " ms" << endl;

    img->render();
}

void MainWindow::zoomIn() {
    auto started = chrono::high_resolution_clock::now();
    img->zoomIn();
    auto done = chrono::high_resolution_clock::now();
    cout << "Zoom in: " << chrono::duration_cast<chrono::milliseconds>(done-started).count() << " ms" << endl;

    img->render();
}

void MainWindow::convolve(Kernel kernel, bool grayBackground) {
    auto started = chrono::high_resolution_clock::now();
    img->convolve(kernel, grayBackground);
    auto done = chrono::high_resolution_clock::now();
    cout << "Convolve: " << chrono::duration_cast<chrono::milliseconds>(done-started).count() << " ms" << endl;

    img->render();
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "controlswrapper.h"
#include "image.h"
#include "imagewindow.h"

#include <QMainWindow>
#include <QLabel>
#include <QSpinBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

// > Properties

private:
    Ui::MainWindow *ui;

    ControlsWrapper *controls;

    Image *img = nullptr;
    Image *origImg = nullptr;

// > Methods

private:
    void closeEvent(QCloseEvent *event);

private slots:
    bool openImgFile(const char *filename);
    void saveImg();
    void copyOriginalImg();
    void hflipImg();
    void vflipImg();
    void grayscaleImg();
    void quantizeImg(int n);
    void showImgHistogram();
    void adjustBrightness(int brightness);
    void adjustContrast(double contrast);
    void negativeImg();
    void equalizeHistogram();
    void matchHistogram(const char *targetFilename);
    void rotate(RotationDirection direction);
    void zoomOut(int sx, int sy);
    void zoomIn();
    void convolve(Kernel kernel, bool grayBackground);

    void origImgWasClosed();
    void editImgWasClosed();

public:
    MainWindow(const char *filename = nullptr);
    ~MainWindow();

};
#endif // MAINWINDOW_H

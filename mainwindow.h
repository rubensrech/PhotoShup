#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "controlswrapper.h"
#include "image.h"
#include "imagelabel.h"

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

    ImageLabel *imgLabel;
    ImageLabel *origImgLabel;

    Image *img = nullptr;
    Image *origImg = nullptr;

// > Methods

private:
    void closeEvent(QCloseEvent *bar);

private slots:
    void openImgFile(const char *filename);
    void saveImg();
    void copyOriginalImg();
    void hflipImg();
    void vflipImg();
    void grayscaleImg();
    void quantizeImg(int n);
    void showImgHistogram();

public:
    MainWindow(const char *filename = nullptr);
    ~MainWindow();

};
#endif // MAINWINDOW_H

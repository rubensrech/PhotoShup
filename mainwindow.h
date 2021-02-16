#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "image.h"
#include "imagelabel.h"

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

// > Properties

private:
    Image *img;
    Image *origImg;

    ImageLabel *imgLabel;
    ImageLabel *origImgLabel;

// > Methods

public:
    MainWindow(const char *filename = nullptr);
    ~MainWindow();

    // > Button event handlers
    void handleHFlipClicked();
    void handleVFlipClicked();
    void handleGrayscaleClicked();
    void handleQuantizeClicked();
    void handleCopyClicked();
    void handleSaveClicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

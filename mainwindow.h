#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

    Image *img = nullptr;
    Image *origImg = nullptr;

    vector<QWidget*> controls;
    ImageLabel *imgLabel;
    ImageLabel *origImgLabel;
    QSpinBox *quantizeValBox;

// > Methods

private:
    void setControlsDisabled(bool disabled);

    // > Button event handlers
    void handleHFlipClicked();
    void handleVFlipClicked();
    void handleGrayscaleClicked();
    void handleQuantizeClicked();
    void handleCopyClicked();
    void handleSaveClicked();
    void handleOpenClicked();

public:
    MainWindow(const char *filename = nullptr);
    ~MainWindow();

};
#endif // MAINWINDOW_H

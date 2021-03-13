#ifndef CONTROLSWRAPPER_H
#define CONTROLSWRAPPER_H

#include "util.h"
#include "kernel.h"

#include <vector>
using namespace std;

#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QWidget>
#include <QCheckBox>

class ControlsWrapper : public QWidget {
    Q_OBJECT

// > Properties

private:
    vector<QWidget*> disablingControls;
    QSpinBox *quantizeValBox;
    QSlider *brightnessSlider;
    QDoubleSpinBox *contrastValBox;
    QSpinBox *zoomOutSxValBox;
    QSpinBox *zoomOutSyValBox;
    vector<vector<QDoubleSpinBox*>> kernelValBoxes;
    QCheckBox *grayBgCheckbox;

// > Methods

private:
    QPushButton *createButton(QString title, QWidget *parent, bool disabling = true);

    QWidget *createImgFileControls(QWidget *parent);

    QWidget *createHistogramControls(QWidget *parent);
    QWidget *createQuantizationControls(QWidget *parent);
    QWidget *createBrightnessControls(QWidget *parent);
    QWidget *createContrastControls(QWidget *parent);
    QWidget *createRotationControls(QWidget *parent);
    QWidget *createZoomControls(QWidget *parent);
    QWidget *createConvolutionControls(QWidget *parent);
    QWidget *createImgProcessingControls(QWidget *parent);

    QString selectImageFile(QString caption);

public:
    explicit ControlsWrapper(QWidget *parent = nullptr);

    void setDisabled(bool disabled = true);

public slots:
    // > Button event handlers

    void handleOpenClicked();
    void handleMatchHistogramClicked();
    void handleQuantizeClicked();
    void handleBrightnessClicked();
    void handleContrastClicked();
    void handleZoomOutClicked();
    void handlePredefKernelClicked(KernelName kernelName);
    void handleConvolveClicked();

signals:
    void openClicked(const char *filename);
    void saveClicked();
    void copyClicked();
    void hflipClicked();
    void vflipClicked();
    void grayscaleClicked();
    void negativeClicked();
    void showHistogramClicked();
    void equalizeHistogramClicked();
    void matchHistogramClicked(const char *filename);
    void quantizeClicked(int n);
    void brightnessClicked(int brightness);
    void contrastClicked(double contrast);
    void rotateClicked(RotationDirection direction);
    void zoomOutClicked(int sx, int sy);
    void zoomInClicked();
    void convolveClicked(Kernel kernel, bool grayBackground);

};

#endif // CONTROLSWRAPPER_H

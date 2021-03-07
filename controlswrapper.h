#ifndef CONTROLSWRAPPER_H
#define CONTROLSWRAPPER_H

#include <vector>
using namespace std;

#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QWidget>

class ControlsWrapper : public QWidget {
    Q_OBJECT

// > Properties

private:
    vector<QWidget*> disablingControls;
    QSpinBox *quantizeValBox;
    QSlider *brightnessSlider;
    QDoubleSpinBox *contrastValBox;

// > Methods

private:
    QPushButton *createButton(QString title, QWidget *parent, bool disabling = true);

    QWidget *createImgFileControls(QWidget *parent);

    QWidget *createQuantizationControls(QWidget *parent);
    QWidget *createBrightnessControls(QWidget *parent);
    QWidget *createContrastControls(QWidget *parent);
    QWidget *createImgProcessingControls(QWidget *parent);

public:
    explicit ControlsWrapper(QWidget *parent = nullptr);

    void setDisabled(bool disabled = true);

public slots:
    // > Button event handlers

    void handleOpenClicked();
    void handleQuantizeClicked();
    void handleBrightnessClicked();
    void handleContrastClicked();

signals:
    void imgFileSelected(const char *filename);
    void saveClicked();
    void copyClicked();
    void hflipClicked();
    void vflipClicked();
    void grayscaleClicked();
    void quantizeClicked(int n);
    void histogramClicked();
    void brightnessClicked(int brightness);
    void contrastClicked(double contrast);
    void negativeClicked();

};

#endif // CONTROLSWRAPPER_H

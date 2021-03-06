#ifndef CONTROLSWRAPPER_H
#define CONTROLSWRAPPER_H

#include <vector>
using namespace std;

#include <QSpinBox>
#include <QWidget>

class ControlsWrapper : public QWidget {
    Q_OBJECT

// > Properties

private:
    vector<QWidget*> disablingControls;
    QSpinBox *quantizeValBox;

// > Methods

public:
    explicit ControlsWrapper(QWidget *parent = nullptr);

    void setDisabled(bool disabled = true);

public slots:
    // > Button event handlers
    void handleOpenClicked();
    void handleCopyClicked();
    void handleHFlipClicked();
    void handleVFlipClicked();
    void handleGrayscaleClicked();
    void handleQuantizeClicked();
    void handleSaveClicked();


signals:
    void imgFileSelected(const char *filename);
    void copyClicked();
    void hflipClicked();
    void vflipClicked();
    void grayscaleClicked();
    void quantizeClicked(int n);
    void saveClicked();


};

#endif // CONTROLSWRAPPER_H

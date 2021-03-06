#include "controlswrapper.h"

#include <QFileDialog>
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>

ControlsWrapper::ControlsWrapper(QWidget *parent) : QWidget(parent) {

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignHCenter);

    // 1. Image File group
    QGroupBox *imgFileGroup = new QGroupBox("Image File", this);
    QVBoxLayout *imgFileGroupLayout = new QVBoxLayout(imgFileGroup);
    imgFileGroup->setLayout(imgFileGroupLayout);
    layout->addWidget(imgFileGroup);

    // 1.1. Open button
    QPushButton *openButton = new QPushButton("Open", this);
    connect(openButton, &QPushButton::clicked, this, &ControlsWrapper::handleOpenClicked);
    imgFileGroupLayout->addWidget(openButton);

    // 1.2. Save button
    QPushButton *saveButton = new QPushButton("Save", this);
    connect(saveButton, &QPushButton::clicked, this, &ControlsWrapper::saveClicked);
    imgFileGroupLayout->addWidget(saveButton);
    disablingControls.push_back(saveButton);

    // 2. Image Processing group
    QGroupBox *imgProcessGroup = new QGroupBox("Image Processing", this);
    QVBoxLayout *imgProcessGroupLayout = new QVBoxLayout(imgProcessGroup);
    imgProcessGroup->setLayout(imgProcessGroupLayout);
    layout->addWidget(imgProcessGroup);

    // 2.1. Copy button
    QPushButton *copyButton = new QPushButton("Copy original", this);
    connect(copyButton, &QPushButton::clicked, this, &ControlsWrapper::copyClicked);
    imgProcessGroupLayout->addWidget(copyButton);
    disablingControls.push_back(copyButton);

    // 2.2. HFlip button
    QPushButton *hFlipButton = new QPushButton("Flip Horizontally", imgProcessGroup);
    imgProcessGroupLayout->addWidget(hFlipButton);
    connect(hFlipButton, &QPushButton::clicked, this, &ControlsWrapper::hflipClicked);
    disablingControls.push_back(hFlipButton);

    // 2.3. VFlip button
    QPushButton *vFlipButton = new QPushButton("Flip Vertically", imgProcessGroup);
    imgProcessGroupLayout->addWidget(vFlipButton);
    connect(vFlipButton, &QPushButton::clicked, this, &ControlsWrapper::vflipClicked);
    disablingControls.push_back(vFlipButton);

    // 2.4. Grayscale button
    QPushButton *grayscaleButton = new QPushButton("Grayscale", imgProcessGroup);
    imgProcessGroupLayout->addWidget(grayscaleButton);
    connect(grayscaleButton, &QPushButton::clicked, this, &ControlsWrapper::grayscaleClicked);
    disablingControls.push_back(grayscaleButton);

    // 2.5. Quantize button
    quantizeValBox = new QSpinBox(imgProcessGroup);
    quantizeValBox->setRange(0, 256);
    quantizeValBox->setSingleStep(10);
    quantizeValBox->setValue(256);
    imgProcessGroupLayout->addWidget(quantizeValBox);
    disablingControls.push_back(quantizeValBox);
    QPushButton *quantizeButton = new QPushButton("Quantize", imgProcessGroup);
    imgProcessGroupLayout->addWidget(quantizeButton);
    connect(quantizeButton, &QPushButton::clicked, this, &ControlsWrapper::handleQuantizeClicked);
    disablingControls.push_back(quantizeButton);

    // 2.6. Histogram button
    QPushButton *histogramButton = new QPushButton("Histogram", imgProcessGroup);
    imgProcessGroupLayout->addWidget(histogramButton);
    connect(histogramButton, &QPushButton::clicked, this, &ControlsWrapper::histogramClicked);
    disablingControls.push_back(histogramButton);

}

void ControlsWrapper::setDisabled(bool disabled) {
    for (QWidget *c : disablingControls) {
        c->setDisabled(disabled);
    }
}

void ControlsWrapper::handleOpenClicked() {
    QString filename = QFileDialog::getOpenFileName(this, "Open image", nullptr, "Images (*.jpg)");
    if (!filename.isEmpty() && !filename.isNull()) {
        emit imgFileSelected(filename.toStdString().c_str());
    }
}

void ControlsWrapper::handleQuantizeClicked() {
    int n = quantizeValBox->value();
    emit quantizeClicked(n);
}


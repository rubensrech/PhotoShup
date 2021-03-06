#include "controlswrapper.h"

#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#define DFT_SPACING 5
#define DFT_MARGIN  5

ControlsWrapper::ControlsWrapper(QWidget *parent) : QWidget(parent) {

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignHCenter);

    // 1. Image File group
    QGroupBox *imgFileGroup = new QGroupBox("Image File", this);
    QVBoxLayout *imgFileGroupLayout = new QVBoxLayout(imgFileGroup);
    imgFileGroupLayout->setSpacing(DFT_SPACING);
    imgFileGroupLayout->setMargin(DFT_MARGIN);
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
    imgProcessGroupLayout->setSpacing(DFT_SPACING);
    imgProcessGroupLayout->setMargin(DFT_MARGIN);
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

    // 2.5. Quantization group
    QGroupBox *quantGroup = new QGroupBox("Quantization", imgProcessGroup);
    QVBoxLayout *quantGroupLayout = new QVBoxLayout(quantGroup);
    quantGroupLayout->setSpacing(DFT_SPACING);
    quantGroupLayout->setMargin(DFT_MARGIN);
    quantGroup->setLayout(quantGroupLayout);
    imgProcessGroupLayout->addWidget(quantGroup);
    // 2.5.1. Quantization row
    QWidget *quantRow = new QWidget(quantGroup);
    QHBoxLayout *quantRowLayout = new QHBoxLayout(quantRow);
    quantRowLayout->setSpacing(DFT_SPACING);
    quantRowLayout->setMargin(DFT_MARGIN);
    quantRow->setLayout(quantRowLayout);
    quantGroupLayout->addWidget(quantRow);
    // 2.5.1.1. Quantization label
    QLabel *quantLabel = new QLabel("Colors: ", quantGroup);
    quantRowLayout->addWidget(quantLabel);
    // 2.5.1.2. Quantization spin box
    quantizeValBox = new QSpinBox(quantGroup);
    quantizeValBox->setRange(0, 256);
    quantizeValBox->setSingleStep(10);
    quantizeValBox->setValue(256);
    quantRowLayout->addWidget(quantizeValBox);
    disablingControls.push_back(quantizeValBox);
    // 2.5.2. Quantization button
    QPushButton *quantButton = new QPushButton("Quantize", quantGroup);
    quantGroupLayout->addWidget(quantButton);
    connect(quantButton, &QPushButton::clicked, this, &ControlsWrapper::handleQuantizeClicked);
    disablingControls.push_back(quantButton);

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


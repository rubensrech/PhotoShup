#include "controlswrapper.h"

#include <QFileDialog>
#include <QPushButton>
#include <QVBoxLayout>

#define CONTROLS_WIDTH 200

ControlsWrapper::ControlsWrapper(QWidget *parent) : QWidget(parent) {

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop);

    // -- Open button
    QPushButton *openButton = new QPushButton("Open", this);
    openButton->setFixedWidth(CONTROLS_WIDTH);
    connect(openButton, &QPushButton::clicked, this, &ControlsWrapper::handleOpenClicked);
    layout->addWidget(openButton);

    // -- Copy button
    QPushButton *copyButton = new QPushButton("Copy original", this);
    copyButton->setFixedWidth(CONTROLS_WIDTH);
    connect(copyButton, &QPushButton::clicked, this, &ControlsWrapper::handleCopyClicked);
    layout->addWidget(copyButton);
    disablingControls.push_back(copyButton);

    // -- HFlip button
    QPushButton *hFlipButton = new QPushButton("Flip Horizontally", this);
    hFlipButton->setFixedWidth(CONTROLS_WIDTH);
    connect(hFlipButton, &QPushButton::clicked, this, &ControlsWrapper::handleHFlipClicked);
    layout->addWidget(hFlipButton);
    disablingControls.push_back(hFlipButton);

    // -- VFlip button
    QPushButton *vFlipButton = new QPushButton("Flip Vertically", this);
    vFlipButton->setFixedWidth(CONTROLS_WIDTH);
    connect(vFlipButton, &QPushButton::clicked, this, &ControlsWrapper::handleVFlipClicked);
    layout->addWidget(vFlipButton);
    disablingControls.push_back(vFlipButton);

    // -- Grayscale button
    QPushButton *grayscaleButton = new QPushButton("Grayscale", this);
    grayscaleButton->setFixedWidth(CONTROLS_WIDTH);
    connect(grayscaleButton, &QPushButton::clicked, this, &ControlsWrapper::handleGrayscaleClicked);
    layout->addWidget(grayscaleButton);
    disablingControls.push_back(grayscaleButton);

    // -- Quantize button
    quantizeValBox = new QSpinBox(this);
    quantizeValBox->setFixedWidth(CONTROLS_WIDTH);
    quantizeValBox->setRange(0, 256);
    quantizeValBox->setSingleStep(10);
    quantizeValBox->setValue(256);
    layout->addWidget(quantizeValBox);
    disablingControls.push_back(quantizeValBox);
    QPushButton *quantizeButton = new QPushButton("Quantize", this);
    quantizeButton->setFixedWidth(CONTROLS_WIDTH);
    connect(quantizeButton, &QPushButton::clicked, this, &ControlsWrapper::handleQuantizeClicked);
    layout->addWidget(quantizeButton);
    disablingControls.push_back(quantizeButton);

    // -- Save button
    QPushButton *saveButton = new QPushButton("Save", this);
    saveButton->setFixedWidth(CONTROLS_WIDTH);
    connect(saveButton, &QPushButton::clicked, this, &ControlsWrapper::handleSaveClicked);
    layout->addWidget(saveButton);
    disablingControls.push_back(saveButton);

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

void ControlsWrapper::handleCopyClicked() {
    emit copyClicked();
}

void ControlsWrapper::handleHFlipClicked() {
    emit hflipClicked();
}

void ControlsWrapper::handleVFlipClicked() {
    emit vflipClicked();
}

void ControlsWrapper::handleGrayscaleClicked() {
    emit grayscaleClicked();
}

void ControlsWrapper::handleQuantizeClicked() {
    int n = quantizeValBox->value();
    emit quantizeClicked(n);
}

void ControlsWrapper::handleSaveClicked() {
    emit saveClicked();
}







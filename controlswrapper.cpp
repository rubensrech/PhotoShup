#include "controlswrapper.h"

#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>

#define DFT_SPACING 5
#define DFT_MARGIN  5


QPushButton *ControlsWrapper::createButton(QString title, QWidget *parent, bool disabling) {
    QPushButton *button = new QPushButton(title, parent);
    if (disabling) {
        disablingControls.push_back(button);
    }
    return button;
}

QWidget *ControlsWrapper::createImgFileControls(QWidget *parent) {
    QGroupBox *group = new QGroupBox("Image File", parent);

    QVBoxLayout *layout = new QVBoxLayout(group);
    layout->setSpacing(DFT_SPACING);
    layout->setMargin(DFT_MARGIN);

    group->setLayout(layout);

    // 1. Open button
    QPushButton *openButton = createButton("Open", this, false);
    connect(openButton, &QPushButton::clicked, this, &ControlsWrapper::handleOpenClicked);
    layout->addWidget(openButton);

    // 2. Save button
    QPushButton *saveButton = createButton("Save", this);
    connect(openButton, &QPushButton::clicked, this, &ControlsWrapper::saveClicked);
    layout->addWidget(saveButton);

    return group;
}

QWidget *ControlsWrapper::createQuantizationControls(QWidget *parent) {
    QGroupBox *group = new QGroupBox("Quantization", parent);

    QVBoxLayout *layout = new QVBoxLayout(group);
    layout->setSpacing(DFT_SPACING);
    layout->setMargin(DFT_MARGIN);

    group->setLayout(layout);

    // 1. Quantization row
    QWidget *row = new QWidget(group);

    QHBoxLayout *rowLayout = new QHBoxLayout(row);
    rowLayout->setSpacing(DFT_SPACING);
    rowLayout->setMargin(DFT_MARGIN);

    row->setLayout(rowLayout);
    layout->addWidget(row);

    // 1.1. Quantization label
    QLabel *label = new QLabel("Colors: ", row);
    rowLayout->addWidget(label);

    // 1.2. Quantization spin box
    quantizeValBox = new QSpinBox(row);
    quantizeValBox->setRange(0, 256);
    quantizeValBox->setSingleStep(10);
    quantizeValBox->setValue(256);
    rowLayout->addWidget(quantizeValBox);
    disablingControls.push_back(quantizeValBox);

    // 2. Quantization button
    QPushButton *quantButton = createButton("Quantize", group);
    connect(quantButton, &QPushButton::clicked, this, &ControlsWrapper::handleQuantizeClicked);
    layout->addWidget(quantButton);

    return group;
}


QWidget *ControlsWrapper::createImgProcessingControls(QWidget *parent) {
    QGroupBox *group = new QGroupBox("Image Processing", parent);

    QVBoxLayout *layout = new QVBoxLayout(group);
    layout->setSpacing(DFT_SPACING);
    layout->setMargin(DFT_MARGIN);

    group->setLayout(layout);

    // 1. Copy button
    QPushButton *copyButton = createButton("Copy original", group);
    connect(copyButton, &QPushButton::clicked, this, &ControlsWrapper::copyClicked);
    layout->addWidget(copyButton);

    // 2. HFlip button
    QPushButton *hFlipButton = createButton("Flip Horizontally", group);
    connect(hFlipButton, &QPushButton::clicked, this, &ControlsWrapper::hflipClicked);
    layout->addWidget(hFlipButton);

    // 3. VFlip button
    QPushButton *vFlipButton = createButton("Flip Vertically", group);
    connect(vFlipButton, &QPushButton::clicked, this, &ControlsWrapper::vflipClicked);
    layout->addWidget(vFlipButton);

    // 4. Grayscale button
    QPushButton *grayscaleButton = createButton("Grayscale", group);
    connect(grayscaleButton, &QPushButton::clicked, this, &ControlsWrapper::grayscaleClicked);
    layout->addWidget(grayscaleButton);

    // 5. Quantization group
    layout->addWidget(createQuantizationControls(group));

    // 6. Histogram button
    QPushButton *histogramButton = createButton("Histogram", group);
    connect(histogramButton, &QPushButton::clicked, this, &ControlsWrapper::histogramClicked);
    layout->addWidget(histogramButton);

    return group;
}

ControlsWrapper::ControlsWrapper(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignHCenter);

    // 1. Image File controls
    layout->addWidget(createImgFileControls(this));

    // 2. Image Processing controls
    layout->addWidget(createImgProcessingControls(this));
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


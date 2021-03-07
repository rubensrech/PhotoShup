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
    connect(saveButton, &QPushButton::clicked, this, &ControlsWrapper::saveClicked);
    layout->addWidget(saveButton);

    return group;
}

QWidget *ControlsWrapper::createHistogramControls(QWidget *parent) {
    QGroupBox *group = new QGroupBox("Histogram", parent);

    QVBoxLayout *layout = new QVBoxLayout(group);
    layout->setSpacing(DFT_SPACING);
    layout->setMargin(DFT_MARGIN);

    group->setLayout(layout);

    // 1. Show Histogram button
    QPushButton *showHistButton = createButton("Show Histogram", group);
    connect(showHistButton, &QPushButton::clicked, this, &ControlsWrapper::showHistogramClicked);
    layout->addWidget(showHistButton);

    // 2. Equalize Histogram button
    QPushButton *eqHistButton = createButton("Equalize Histogram", group);
    connect(eqHistButton, &QPushButton::clicked, this, &ControlsWrapper::equalizeHistogramClicked);
    layout->addWidget(eqHistButton);

    // 3. Match Histogram button
    QPushButton *matchHistButton = createButton("Match Histogram", group);
    connect(matchHistButton, &QPushButton::clicked, this, &ControlsWrapper::handleMatchHistogramClicked);
    layout->addWidget(matchHistButton);

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

QWidget *ControlsWrapper::createBrightnessControls(QWidget *parent) {
    QGroupBox *group = new QGroupBox("Brightness", parent);

    QVBoxLayout *layout = new QVBoxLayout(group);
    layout->setSpacing(DFT_SPACING);
    layout->setMargin(DFT_MARGIN);

    group->setLayout(layout);

    // 1. Brightness slider
    brightnessSlider = new QSlider(group);
    brightnessSlider->setOrientation(Qt::Horizontal);
    brightnessSlider->setRange(-255, 255);
    brightnessSlider->setTickPosition(QSlider::TicksBelow);
    brightnessSlider->setTickInterval(255);
    layout->addWidget(brightnessSlider);
    disablingControls.push_back(brightnessSlider);

    // 2. Brightness button
    QPushButton *button = createButton("Adjust brightness", group);
    connect(button, &QPushButton::clicked, this, &ControlsWrapper::handleBrightnessClicked);
    layout->addWidget(button);

    return group;
}

QWidget *ControlsWrapper::createContrastControls(QWidget *parent) {
    QGroupBox *group = new QGroupBox("Contrast", parent);

    QVBoxLayout *layout = new QVBoxLayout(group);
    layout->setSpacing(DFT_SPACING);
    layout->setMargin(DFT_MARGIN);

    group->setLayout(layout);

    // 1. Contrast double spin box
    contrastValBox = new QDoubleSpinBox(group);
    contrastValBox->setRange(0.1, 255.0);
    contrastValBox->setSingleStep(0.1);
    contrastValBox->setValue(1.0);
    contrastValBox->setDecimals(1);
    layout->addWidget(contrastValBox);
    disablingControls.push_back(contrastValBox);

    // 2. Contrast button
    QPushButton *button = createButton("Adjust constrast", group);
    connect(button, &QPushButton::clicked, this, &ControlsWrapper::handleContrastClicked);
    layout->addWidget(button);

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

    // 5. Negative button
    QPushButton *negativeButton = createButton("Negative", group);
    connect(negativeButton, &QPushButton::clicked, this, &ControlsWrapper::negativeClicked);
    layout->addWidget(negativeButton);

    // 6. Histogram group
    layout->addWidget(createHistogramControls(group));

    // 7. Quantization group
    layout->addWidget(createQuantizationControls(group));

    // 8. Brightness group
    layout->addWidget(createBrightnessControls(group));

    // 9. Constrast group
    layout->addWidget(createContrastControls(group));

    return group;
}

QString ControlsWrapper::selectImageFile(QString caption) {
    return QFileDialog::getOpenFileName(this, caption, nullptr, "Images (*.jpg)");
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
    QString filename = selectImageFile("Open image");
    if (!filename.isEmpty() && !filename.isNull()) {
        emit openClicked(filename.toStdString().c_str());
    }
}

void ControlsWrapper::handleMatchHistogramClicked() {
    QString filename = selectImageFile("Select image whose histogram should be matched");
    if (!filename.isEmpty() && !filename.isNull()) {
        emit matchHistogramClicked(filename.toStdString().c_str());
    }
}

void ControlsWrapper::handleQuantizeClicked() {
    int n = quantizeValBox->value();
    emit quantizeClicked(n);
}

void ControlsWrapper::handleBrightnessClicked() {
    int brightness = brightnessSlider->value();
    emit brightnessClicked(brightness);
}

void ControlsWrapper::handleContrastClicked() {
    double contrast = contrastValBox->value();
    emit contrastClicked(contrast);
}


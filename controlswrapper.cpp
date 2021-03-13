#include "controlswrapper.h"

#include <QFileDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QVBoxLayout>

#include "kernel.h"

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
    QPushButton *showHistButton = createButton("Show", group);
    connect(showHistButton, &QPushButton::clicked, this, &ControlsWrapper::showHistogramClicked);
    layout->addWidget(showHistButton);

    // 2. Equalize Histogram button
    QPushButton *eqHistButton = createButton("Equalize", group);
    connect(eqHistButton, &QPushButton::clicked, this, &ControlsWrapper::equalizeHistogramClicked);
    layout->addWidget(eqHistButton);

    // 3. Match Histogram button
    QPushButton *matchHistButton = createButton("Match ...", group);
    connect(matchHistButton, &QPushButton::clicked, this, &ControlsWrapper::handleMatchHistogramClicked);
    layout->addWidget(matchHistButton);

    return group;
}

QWidget *ControlsWrapper::createQuantizationControls(QWidget *parent) {
    QGroupBox *group = new QGroupBox("Quantization", parent);

    QFormLayout *layout = new QFormLayout(group);
    layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    layout->setSpacing(DFT_SPACING);
    layout->setMargin(DFT_MARGIN);

    group->setLayout(layout);

    // 1. Colors spin box
    quantizeValBox = new QSpinBox(group);
    quantizeValBox->setRange(0, 256);
    quantizeValBox->setSingleStep(10);
    quantizeValBox->setValue(256);
    layout->addRow("Colors:", quantizeValBox);
    disablingControls.push_back(quantizeValBox);


    // 2. Quantize button
    QPushButton *quantButton = createButton("Quantize", group);
    connect(quantButton, &QPushButton::clicked, this, &ControlsWrapper::handleQuantizeClicked);
    layout->addRow(quantButton);

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
    QPushButton *button = createButton("Adjust", group);
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
    QPushButton *button = createButton("Adjust", group);
    connect(button, &QPushButton::clicked, this, &ControlsWrapper::handleContrastClicked);
    layout->addWidget(button);

    return group;
}

QWidget *ControlsWrapper::createRotationControls(QWidget *parent) {
    QGroupBox *group = new QGroupBox("Rotate", parent);

    QVBoxLayout *layout = new QVBoxLayout(group);
    layout->setSpacing(DFT_SPACING);
    layout->setMargin(DFT_MARGIN);

    group->setLayout(layout);

    // 1. Clockwise button
    QPushButton *clkwiseButton = createButton("Clockwise", group);
    connect(clkwiseButton, &QPushButton::clicked, [this]{ rotateClicked(ClockWise); });
    layout->addWidget(clkwiseButton);

    // 2. Counter-clockwise button
    QPushButton *counterClkwiseButton = createButton("Counter-clk", group);
    connect(counterClkwiseButton, &QPushButton::clicked, [this]{ rotateClicked(CounterClockwise); });
    layout->addWidget(counterClkwiseButton);

    return group;
}

QWidget *ControlsWrapper::createZoomControls(QWidget *parent) {
    QGroupBox *group = new QGroupBox("Zoom", parent);

    QFormLayout *layout = new QFormLayout(group);
    layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    layout->setSpacing(DFT_SPACING);
    layout->setMargin(DFT_MARGIN);

    group->setLayout(layout);

    // 1. Horiz. spin box
    zoomOutSxValBox = new QSpinBox(group);
    zoomOutSxValBox->setMinimum(1);
    zoomOutSxValBox->setSingleStep(1);
    zoomOutSxValBox->setValue(2);
    layout->addRow("Horiz:", zoomOutSxValBox);
    disablingControls.push_back(zoomOutSxValBox);

    // 2. Vert. spin box
    zoomOutSyValBox = new QSpinBox(group);
    zoomOutSyValBox->setMinimum(1);
    zoomOutSyValBox->setSingleStep(1);
    zoomOutSyValBox->setValue(2);
    layout->addRow("Vert:", zoomOutSyValBox);
    disablingControls.push_back(zoomOutSyValBox);

    // 2. Zoom out button
    QPushButton *zoomOutButton = createButton("Zoom out", group);
    connect(zoomOutButton, &QPushButton::clicked, this, &ControlsWrapper::handleZoomOutClicked);
    layout->addRow(zoomOutButton);

    // 3. Separator line
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addRow(line);

    // 4. Zoom in button
    QPushButton *zoomInButton = createButton("Zoom in", group);
    connect(zoomInButton, &QPushButton::clicked, this, &ControlsWrapper::zoomInClicked);
    layout->addRow(zoomInButton);

    return group;
}

QWidget *ControlsWrapper::createConvolutionControls(QWidget *parent) {
    QGroupBox *group = new QGroupBox("Convolution", parent);

    QVBoxLayout *layout = new QVBoxLayout(group);
    layout->setSpacing(DFT_SPACING);
    layout->setMargin(DFT_MARGIN);

    group->setLayout(layout);

    // 1. Predefined kernels group
    QGroupBox *predefGrid = new QGroupBox("Predefined kernels", group);
    QGridLayout *predefGridLayout = new QGridLayout(predefGrid);
    predefGridLayout->setSpacing(2);
    predefGridLayout->setMargin(0);
    predefGrid->setLayout(predefGridLayout);
    layout->addWidget(predefGrid);
    // 1.0.0. Laplacian button
    QPushButton *laplacianButton = createButton("Laplacian", predefGrid);
    connect(laplacianButton, &QPushButton::clicked, [this]{ handlePredefKernelClicked(Laplacian); });
    predefGridLayout->addWidget(laplacianButton, 0, 0);
    // 1.0.1. High Pass button
    QPushButton *highpassButton = createButton("High Pass", predefGrid);
    connect(highpassButton, &QPushButton::clicked, [this]{ handlePredefKernelClicked(HighPass); });
    predefGridLayout->addWidget(highpassButton, 0, 1);
    // 1.1.0. PrewittX button
    QPushButton *prewittXButton = createButton("Prewitt X", predefGrid);
    connect(prewittXButton, &QPushButton::clicked, [this]{ handlePredefKernelClicked(PrewittX); });
    predefGridLayout->addWidget(prewittXButton, 1, 0);
    // 1.1.1. PrewittY button
    QPushButton *prewittYButton = createButton("Prewitt Y", predefGrid);
    connect(prewittYButton, &QPushButton::clicked, [this]{ handlePredefKernelClicked(PrewittY); });
    predefGridLayout->addWidget(prewittYButton, 1, 1);
    // 1.2.0. Sobel X button
    QPushButton *sobelXButton = createButton("Sobel X", predefGrid);
    connect(sobelXButton, &QPushButton::clicked, [this]{ handlePredefKernelClicked(SobelX); });
    predefGridLayout->addWidget(sobelXButton, 2, 0);
    // 1.2.1. Sobel Y button
    QPushButton *sobelYButton = createButton("Sobel Y", predefGrid);
    connect(sobelYButton, &QPushButton::clicked, [this]{ handlePredefKernelClicked(SobelY); });
    predefGridLayout->addWidget(sobelYButton, 2, 1);
    // 1.3.0. Gaussian button
    QPushButton *gaussianButton = createButton("Gaussian", predefGrid);
    connect(gaussianButton, &QPushButton::clicked, [this]{ handlePredefKernelClicked(Gaussian); });
    predefGridLayout->addWidget(gaussianButton, 3, 0);
    // 1.3.1. Gray Background checkbox
    grayBgCheckbox = new QCheckBox("Gray bkgnd", predefGrid);
    disablingControls.push_back(grayBgCheckbox);
    predefGridLayout->addWidget(grayBgCheckbox, 3, 1, Qt::AlignCenter);

    // 2. Kernel values grid
    QWidget *kernelValsGrid = new QWidget(group);
    QGridLayout *kernelValsGridLayout = new QGridLayout(kernelValsGrid);
    kernelValsGridLayout->setSpacing(2);
    kernelValsGridLayout->setMargin(0);
    kernelValsGrid->setLayout(kernelValsGridLayout);
    layout->addWidget(kernelValsGrid);

    kernelValBoxes.reserve(KERNEL_SIZE);
    for (int y = 0; y < KERNEL_SIZE; y++) {
        kernelValBoxes[y].reserve(KERNEL_SIZE);
        for (int x = 0; x < KERNEL_SIZE; x++) {
            // 1.y.x. Kernel values spin box
            QDoubleSpinBox *spinBox = new QDoubleSpinBox(group);
            spinBox->setSingleStep(0.1);
            spinBox->setDecimals(4);
            spinBox->setMaximumWidth(70);
            spinBox->setValue(0.1111);
            spinBox->setRange(-100, 100);
            kernelValsGridLayout->addWidget(spinBox, y, x);
            kernelValBoxes[y][x] = spinBox;
            disablingControls.push_back(spinBox);
        }
    }

    // 3. Convolution button
    QPushButton *convButton = createButton("Convolve", group);
    connect(convButton, &QPushButton::clicked, this, &ControlsWrapper::handleConvolveClicked);
    layout->addWidget(convButton);

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

    // 6. Grid
    QWidget *grid = new QWidget(group);
    QGridLayout *gridLayout = new QGridLayout(grid);
    gridLayout->setMargin(0);
    gridLayout->setSpacing(0);
    grid->setLayout(gridLayout);
    layout->addWidget(grid);

    // 6.0.0. Brightness group
    gridLayout->addWidget(createBrightnessControls(grid), 0, 0);

    // 6.0.1. Contrast group
    gridLayout->addWidget(createContrastControls(grid), 0, 1);

    // 6.1.0. Rotate group
    gridLayout->addWidget(createRotationControls(grid), 1, 0);

    // 6.1.1. Quantization group
    gridLayout->addWidget(createQuantizationControls(grid), 1, 1);

    // 6.2.0. Histogram group
    gridLayout->addWidget(createHistogramControls(grid), 2, 0);

    // 6.2.1. Zoom group
    gridLayout->addWidget(createZoomControls(grid), 2, 1);

    // 7. Convolution group
    layout->addWidget(createConvolutionControls(group));

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

void ControlsWrapper::handleZoomOutClicked() {
    int sx = zoomOutSxValBox->value();
    int sy = zoomOutSyValBox->value();
    emit zoomOutClicked(sx, sy);
}

void ControlsWrapper::handlePredefKernelClicked(KernelName kernelName) {
    Kernel kernel = Kernel::getByName(kernelName);
    for (int y = 0; y < KERNEL_SIZE; y++) {
        for (int x = 0; x < KERNEL_SIZE; x++) {
            kernelValBoxes[y][x]->setValue(kernel.at(x, y));
        }
    }
}

void ControlsWrapper::handleConvolveClicked() {
    vector<vector<double>> kernelVals(KERNEL_SIZE, vector<double>(KERNEL_SIZE));
    for (int y = 0; y < KERNEL_SIZE; y++) {
        for (int x = 0; x < KERNEL_SIZE; x++) {
            kernelVals[y][x] = kernelValBoxes[y][x]->value();
        }
    }

    Kernel kernel(kernelVals);
    bool grayBackground = grayBgCheckbox->checkState() == Qt::Checked;
    emit convolveClicked(kernel, grayBackground);
}


#include "imagelabel.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QPushButton>

#include <iostream>
using namespace std;

#define CONTROLS_WIDTH 200

MainWindow::MainWindow(const char *filename):
    QMainWindow(), ui(new Ui::MainWindow) {

    ui->setupUi(this);
    this->setWindowTitle("Image editor");

    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(centralWidget);

    origImgLabel = new ImageLabel("Original image");
    imgLabel = new ImageLabel(centralWidget);

    // - Buttons wrapper
    QWidget *buttonsWrapper = new QWidget(centralWidget);
    QVBoxLayout *buttonsWrapperLayout = new QVBoxLayout(buttonsWrapper);
    buttonsWrapperLayout->setAlignment(Qt::AlignTop);

    // -- Open button
    QPushButton *openButton = new QPushButton("Open", buttonsWrapper);
    openButton->setFixedWidth(CONTROLS_WIDTH);
    connect(openButton, &QPushButton::clicked, this, &MainWindow::handleOpenClicked);
    buttonsWrapperLayout->addWidget(openButton);

    // -- Copy button
    QPushButton *copyButton = new QPushButton("Copy original", buttonsWrapper);
    copyButton->setFixedWidth(CONTROLS_WIDTH);
    connect(copyButton, &QPushButton::clicked, this, &MainWindow::handleCopyClicked);
    buttonsWrapperLayout->addWidget(copyButton);
    controls.push_back(copyButton);

    // -- HFlip button
    QPushButton *hFlipButton = new QPushButton("Flip Horizontally", buttonsWrapper);
    hFlipButton->setFixedWidth(CONTROLS_WIDTH);
    connect(hFlipButton, &QPushButton::clicked, this, &MainWindow::handleHFlipClicked);
    buttonsWrapperLayout->addWidget(hFlipButton);
    controls.push_back(hFlipButton);

    // -- VFlip button
    QPushButton *vFlipButton = new QPushButton("Flip Vertically", buttonsWrapper);
    vFlipButton->setFixedWidth(CONTROLS_WIDTH);
    connect(vFlipButton, &QPushButton::clicked, this, &MainWindow::handleVFlipClicked);
    buttonsWrapperLayout->addWidget(vFlipButton);
    controls.push_back(vFlipButton);

    // -- Grayscale button
    QPushButton *grayscaleButton = new QPushButton("Grayscale", buttonsWrapper);
    grayscaleButton->setFixedWidth(CONTROLS_WIDTH);
    connect(grayscaleButton, &QPushButton::clicked, this, &MainWindow::handleGrayscaleClicked);
    buttonsWrapperLayout->addWidget(grayscaleButton);
    controls.push_back(grayscaleButton);

    // -- Quantize button
    quantizeValBox = new QSpinBox(buttonsWrapper);
    quantizeValBox->setFixedWidth(CONTROLS_WIDTH);
    quantizeValBox->setRange(0, 256);
    quantizeValBox->setSingleStep(10);
    quantizeValBox->setValue(256);
    buttonsWrapperLayout->addWidget(quantizeValBox);
    controls.push_back(quantizeValBox);
    QPushButton *quantizeButton = new QPushButton("Quantize", buttonsWrapper);
    quantizeButton->setFixedWidth(CONTROLS_WIDTH);
    connect(quantizeButton, &QPushButton::clicked, this, &MainWindow::handleQuantizeClicked);
    buttonsWrapperLayout->addWidget(quantizeButton);
    controls.push_back(quantizeButton);

    // -- Save button
    QPushButton *saveButton = new QPushButton("Save", buttonsWrapper);
    saveButton->setFixedWidth(CONTROLS_WIDTH);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::handleSaveClicked);
    buttonsWrapperLayout->addWidget(saveButton);
    controls.push_back(saveButton);

    layout->addWidget(this->imgLabel);
    layout->addWidget(buttonsWrapper);

    this->setCentralWidget(centralWidget);

    if (filename) {
        origImg = new Image(filename);
        img = new Image(filename);

        origImgLabel->setImage(origImg);
        imgLabel->setImage(img);
    } else {
        setControlsDisabled(true);
        setFixedSize(QSize(CONTROLS_WIDTH+40, 330));
    }
}

MainWindow::~MainWindow() {
    delete ui;

    if (origImgLabel) delete origImgLabel;

    if (img) delete img;
    if (origImg) delete origImg;
}

void MainWindow::setControlsDisabled(bool disabled) {
    for (QWidget *c : controls) {
        c->setDisabled(disabled);
    }
}

void MainWindow::handleHFlipClicked() {
    img->flipHorizontally();
    imgLabel->render();
}

void MainWindow::handleVFlipClicked() {
    img->flipVertically();
    imgLabel->render();
}

void MainWindow::handleGrayscaleClicked() {
    img->toGrayScale();
    imgLabel->render();
}

void MainWindow::handleQuantizeClicked() {
    int n = quantizeValBox->value();
    img->quantize(n);
    imgLabel->render();
}

void MainWindow::handleCopyClicked() {
    img->copy(origImg);
    imgLabel->render();
}

void MainWindow::handleSaveClicked() {
    QString origFilename = QString::fromStdString(img->getFilename());
    QString filename = QFileDialog::getSaveFileName(this, "Save image", origFilename);
    img->save(filename.toStdString().c_str());
}

void MainWindow::handleOpenClicked() {
    if (origImg) delete origImg;
    if (img) delete img;

    QString filename = QFileDialog::getOpenFileName(this, "Open image", nullptr, "Images (*.png, *.jpg)");
    if (!filename.isEmpty() && !filename.isNull()) {
        origImg = new Image(filename.toStdString().c_str());
        img = new Image(filename.toStdString().c_str());

        origImgLabel->setImage(origImg);
        imgLabel->setImage(img);

        setControlsDisabled(false);
        setFixedSize(QSize(imgLabel->width()+CONTROLS_WIDTH+56, imgLabel->height()+51));
    }
}

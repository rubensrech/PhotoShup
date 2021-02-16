#include "imagelabel.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHBoxLayout>
#include <QPushButton>

#include <iostream>
using namespace std;

#define IMAGE_HEIGHT 600

MainWindow::MainWindow(const char *filename):
    QMainWindow(), ui(new Ui::MainWindow) {

    ui->setupUi(this);
    this->setWindowTitle("Image editor");

    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(centralWidget);

    if (filename) {
        origImg = new Image(filename);
        origImgLabel = new ImageLabel(origImg, "Original image");

        img = new Image(filename);
        imgLabel = new ImageLabel(img, centralWidget);
    }

    // - Buttons wrapper
    QWidget *buttonsWrapper = new QWidget(centralWidget);
    QVBoxLayout *buttonsWrapperLayout = new QVBoxLayout(buttonsWrapper);
    buttonsWrapperLayout->setAlignment(Qt::AlignTop);

    // -- Copy button
    QPushButton *copyButton = new QPushButton("Copy original", buttonsWrapper);
    connect(copyButton, &QPushButton::clicked, this, &MainWindow::handleCopyClicked);
    buttonsWrapperLayout->addWidget(copyButton);

    // -- HFlip button
    QPushButton *hFlipButton = new QPushButton("Flip Horizontally", buttonsWrapper);
    connect(hFlipButton, &QPushButton::clicked, this, &MainWindow::handleHFlipClicked);
    buttonsWrapperLayout->addWidget(hFlipButton);

    // -- VFlip button
    QPushButton *vFlipButton = new QPushButton("Flip Vertically", buttonsWrapper);
    connect(vFlipButton, &QPushButton::clicked, this, &MainWindow::handleVFlipClicked);
    buttonsWrapperLayout->addWidget(vFlipButton);

    // -- Grayscale button
    QPushButton *grayscaleButton = new QPushButton("Grayscale", buttonsWrapper);
    connect(grayscaleButton, &QPushButton::clicked, this, &MainWindow::handleGrayscaleClicked);
    buttonsWrapperLayout->addWidget(grayscaleButton);

    // -- Quantize button
    QPushButton *quantizeButton = new QPushButton("Quantize", buttonsWrapper);
    connect(quantizeButton, &QPushButton::clicked, this, &MainWindow::handleQuantizeClicked);
    buttonsWrapperLayout->addWidget(quantizeButton);

    layout->addWidget(this->imgLabel);
    layout->addWidget(buttonsWrapper);

    this->setCentralWidget(centralWidget);
}

MainWindow::~MainWindow() {
    delete ui;

    if (origImgLabel) delete origImgLabel;

    if (img) delete img;
    if (origImg) delete origImg;
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
    img->quantize(10);
    imgLabel->render();
}

void MainWindow::handleCopyClicked() {
    cout << "Copy" << endl;
}

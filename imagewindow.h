#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include <QWidget>
#include <QLabel>

class ImageWindow : public QLabel {
    Q_OBJECT

private:
    void closeEvent(QCloseEvent *event);

public:
    ImageWindow(QString windowTitle);
    ~ImageWindow();

signals:
    void onClose();

};

#endif // IMAGEWINDOW_H

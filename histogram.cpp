#include "histogram.h"

Histogram::Histogram() {
    _histogram = vector<int>(256, 0);
}

int &Histogram::operator[](int index) {
    return _histogram[index];
}

void Histogram::show(QString title) {
    Histogram histogram = *this;

    QBarSet *set = new QBarSet("Count");
    set->setColor(QColor(20, 70, 200));
    for (int i = 0; i < 256; i++) *set << histogram[i];

    QBarSeries *series = new QBarSeries();
    series->append(set);
    series->setBarWidth(1);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(title);

    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Intensity");
    axisX->setRange(0, 255);
    axisX->setLabelFormat("%d");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Count");
    axisY->setLabelFormat("%d");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(false);

    QChartView *chartView = new QChartView(chart);
    chartView->setAttribute(Qt::WA_DeleteOnClose);
    chartView->setWindowTitle(title);
    chartView->setMinimumSize(QSize(500, 400));
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->show();
}

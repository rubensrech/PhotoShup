#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QtCharts>
using namespace QtCharts;

#include <vector>
using namespace std;

#include <QChartView>

class Histogram {

// > Properties

private:
    vector<int> _histogram;


// > Methods

public:
    Histogram();

    int &operator[](int);

    void show(QString title = "Grayscale Image Histogram");

};

#endif // HISTOGRAM_H

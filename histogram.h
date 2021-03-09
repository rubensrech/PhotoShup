#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QtCharts>
using namespace QtCharts;

#include <vector>
using namespace std;

#include <QChartView>

class Histogram;
class Histogram {

// > Properties

private:
    vector<int> _histogram;


// > Methods

public:
    Histogram();

    template<typename T>
    Histogram(vector<T> v): Histogram() {
        for (int i = 0; i < 256; i++) {
            (*this)[i] = v[i];
        }
    }

    int &operator[](int);

    vector<int> asVector() { return _histogram; }

    Histogram accumulateAndNormalize(float a);

    QChartView *show(QString title = "Grayscale Image Histogram");

};

#endif // HISTOGRAM_H

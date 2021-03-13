#ifndef KERNEL_H
#define KERNEL_H

#include <vector>
using namespace std;

#define KERNEL_SIZE 3

enum KernelName {
    Gaussian,
    Laplacian,
    HighPass,
    PrewittX,
    PrewittY,
    SobelX,
    SobelY,
    Custom,
};

class Kernel {

public:
    static Kernel gaussian() {
        return Kernel({
            { 0.0625, 0.1250, 0.0625 },
            { 0.1250, 0.2500, 0.1250 },
            { 0.0625, 0.1250, 0.0625 },
        }, Gaussian);
    }

    static Kernel laplacian() {
        return Kernel({
            {  0, -1,  0 },
            { -1,  4, -1 },
            {  0, -1,  0 },
        }, Laplacian);
    }

    static Kernel highPass() {
        return Kernel({
            { -1, -1, -1 },
            { -1,  8, -1 },
            { -1, -1, -1 },
        }, HighPass);
    }

    static Kernel prewittX() {
        return Kernel({
            { -1, 0, 1 },
            { -1, 0, 1 },
            { -1, 0, 1 },
        }, PrewittX);
    }

    static Kernel prewittY() {
        return Kernel({
            { -1, -1, -1 },
            {  0,  0,  0 },
            {  1,  1 , 1 },
        }, PrewittY);
    }

    static Kernel sobelX() {
        return Kernel({
            { -1,  0,  1 },
            { -2,  0,  2 },
            { -1,  0 , 1 },
        }, SobelX);
    }

    static Kernel sobelY() {
        return Kernel({
            { -1, -2, -1 },
            {  0,  0,  0 },
            {  1,  2,  1 },
        }, SobelY);
    }

    static Kernel zero() {
        return Kernel({
            { 0, 0, 0 },
            { 0, 0, 0 },
            { 0, 0, 0 },
        }, Custom);
    }

    static Kernel getByName(KernelName name) {
        switch (name) {
        case Gaussian: return gaussian();
        case Laplacian: return laplacian();
        case HighPass: return highPass();
        case PrewittX: return prewittX();
        case PrewittY: return prewittY();
        case SobelX: return sobelX();
        case SobelY: return sobelY();
        default: return zero();
        }
    }

// > Properties

private:
    KernelName _name;
    vector<vector<double>> _values;

// > Methods

public:
    /**
     * @brief Kernel
     * @param name - Kernel name
     * @param values - Matrix ROWS x COLS containing the kernel values
     */
    Kernel(vector<vector<double>> values, KernelName name = Custom) {
        _name = name;
        _values = values;
    }

    double at(int x, int y) { return _values[y][x]; }

    KernelName name() { return _name; }
    int height() { return _values.size(); }
    int width()  { return (height() > 0) ? _values[0].size() : 0; }

};

#endif // KERNEL_H

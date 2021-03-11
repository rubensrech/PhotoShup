#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <algorithm>
using namespace std;

enum RotationDirection {
    ClockWise,
    CounterClockwise,
};

template<typename T>
T clamp(T val, T lower, T upper) {
    return max(lower, min(val, upper));
}

// > Vector functions

template<typename T>
int indexOfNearest(vector<T> vec, T value) {
    typename vector<T>::iterator fst = vec.begin(), lst = vec.end();
    return distance(fst, min_element(fst, lst, [&](T a, T b) {
        return abs(a - value) < abs(b - value);
    }));
}

template<typename T>
T max_element(const vector<T>& vec) {
    return *max_element(vec.begin(), vec.end());
}

// > Vector operators

template<typename vec_t, typename a_t>
vector<vec_t> operator*(const vector<vec_t>& vec, a_t alpha) {
    vector<vec_t> resp(vec.size());
    for (int i = 0; i < vec.size(); i++)
        resp[i] = (a_t)vec[i] * alpha;
    return resp;
}

template<typename vec_t, typename d_t>
vector<vec_t> operator/(const vector<vec_t>& vec, d_t d) {
    vector<vec_t> resp(vec.size());
    for (int i = 0; i < vec.size(); i++)
        resp[i] = (d_t)vec[i] / d;
    return resp;
}

template<typename vec_t, typename a_t>
void operator*=(vector<vec_t>& vec, a_t alpha) {
    for (int i = 0; i < vec.size(); i++)
        vec[i] = (a_t)vec[i] * alpha;
}

#endif // UTIL_H

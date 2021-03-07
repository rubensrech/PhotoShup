#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <algorithm>
using namespace std;

template<typename T>
int indexOfNearest(vector<T> vec, T value) {
    typename vector<T>::iterator fst = vec.begin(), lst = vec.end();
    return distance(fst, min_element(fst, lst, [&](T a, T b) {
        return abs(a - value) < abs(b - value);
    }));
}

template<typename T>
T clamp(T val, T lower, T upper) {
    return max(lower, min(val, upper));
}

#endif // UTIL_H

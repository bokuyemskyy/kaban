#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>
#include <utility>

float eucledianDistance(const std::pair<float, float> &a, const std::pair<float, float> &b);

struct Dimensions {
    int width;
    int height;
};

#endif
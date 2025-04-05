#include "utils.hpp"

float eucledianDistance(const std::pair<float, float> &a, const std::pair<float, float> &b) {
    return std::sqrt(((a.first - b.first) * (a.first - b.first)) +
                     ((a.second - b.second) * (a.second - b.second)));
}
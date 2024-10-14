#pragma once

#include <vector>
#include <glm/glm.hpp>

struct WaveletComponent {
    std::vector<std::vector<float>> coefficients; // 各スケールごとの係数
    int scaleCount;                                // スケール数
};

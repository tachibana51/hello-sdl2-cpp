// src/Components/WaveletComponent.hpp
#pragma once

#include <vector>
#include <glm/glm.hpp>

struct WaveletComponent {
    std::vector<std::vector<float>> detailCoefficients; // 各スケールごとの詳細係数
    std::vector<std::vector<float>> approxCoefficients; // 各スケールごとの近似係数
    int scaleCount;                                     // スケール数
};

// src/Components/WaveletVisualizationComponent.hpp
#pragma once

#include <vector>
#include <glm/glm.hpp>

struct WaveletVisualizationComponent {
    std::vector<glm::vec3> points; // 3D空間上の点群
    std::vector<glm::vec4> colors; // 各点の色（RGBA）
};

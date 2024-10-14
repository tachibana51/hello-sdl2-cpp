
#pragma once

#include <glm/glm.hpp>

struct ProjectionComponent {
    float aspectRatio; // アスペクト比
    float nearPlane;   // 近クリップ面
    float farPlane;    // 遠クリップ面
    float fov;
};

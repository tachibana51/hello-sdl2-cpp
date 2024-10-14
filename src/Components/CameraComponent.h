
#pragma once

#include <glm/glm.hpp>

struct CameraComponent {
    glm::vec3 position; // カメラの位置
    float yaw;          // 経度（ラジアン）
    float pitch;        // 緯度（ラジアン）
    float fov;          // 視野角（ラジアン）
};

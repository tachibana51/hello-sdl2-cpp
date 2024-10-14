
#pragma once

#include <glm/glm.hpp>

struct MorphingComponent {
    glm::vec3 startPosition;   // モーフ開始時の位置
    glm::vec3 targetPosition;  // モーフ終了時の位置
    float duration;            // モーフの総時間（秒）
    float elapsed;             // 経過時間（秒）
};

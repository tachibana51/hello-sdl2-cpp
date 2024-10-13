// PositionComponent.h
#pragma once
#include <glm/glm.hpp>

enum class CoordinateSystem {
    Cartesian,
    Polar
};
template <CoordinateSystem CS>
struct PositionComponent {
    glm::vec3 position;

    PositionComponent(const glm::vec3& pos) : position(pos) {}
    // 座標系の変換
    glm::vec3 getTransformedPosition() const {
        if constexpr (CS == CoordinateSystem::Cartesian) {
            return position;
        } else {
            // 極座標からデカルト座標への変換（例として）
            float r = position.x;
            float theta = position.y;
            float phi = position.z;
            return glm::vec3(
                r * sin(phi) * cos(theta),
                r * sin(phi) * sin(theta),
                r * cos(phi)
            );
        }
    }
};

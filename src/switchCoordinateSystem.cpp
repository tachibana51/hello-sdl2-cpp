// src/switchCoordinateSystem.cpp
#include "switchCoordinateSystem.h"
#include "PositionComponent.h"
#include <memory>
#include <glm/glm.hpp>

void switchCoordinateSystem(std::vector<Entity>& pointCloud, CoordinateSystem newSystem) {
    for (auto& point : pointCloud) {
        if (newSystem == CoordinateSystem::Cartesian) {
            auto polarPosition = point.getComponent<PositionComponent<CoordinateSystem::Polar>>();
            if (polarPosition) {
                // 極座標からデカルト座標に変換
                glm::vec3 cartesianPos = polarPosition->getTransformedPosition();
                // 新しいコンポーネントを追加
                point.addComponent<PositionComponent<CoordinateSystem::Cartesian>>(cartesianPos);
                // 古いコンポーネントを削除
                point.removeComponent<PositionComponent<CoordinateSystem::Polar>>();
            }
        } else if (newSystem == CoordinateSystem::Polar) {
            auto cartesianPosition = point.getComponent<PositionComponent<CoordinateSystem::Cartesian>>();
            if (cartesianPosition) {
                // デカルト座標から極座標に変換
                glm::vec3 pos = cartesianPosition->position;
                float r = glm::length(pos);
                float theta = atan2(pos.y, pos.x);
                float phi = acos(pos.z / r);
                glm::vec3 polarPos(r, theta, phi);
                // 新しいコンポーネントを追加
                point.addComponent<PositionComponent<CoordinateSystem::Polar>>(polarPos);
                // 古いコンポーネントを削除
                point.removeComponent<PositionComponent<CoordinateSystem::Cartesian>>();
            }
        }
    }
}

// src/updatePositions.cpp
#include "updatePositions.h"
#include "PositionComponent.h"
#include "VelocityComponent.h"
#include <glm/glm.hpp>
#include <cmath>
#include "DebugUtils.h"

void updatePositions(std::vector<Entity>& pointCloud) {
    for (auto& point : pointCloud) {
        auto velocity = point.getComponent<VelocityComponent>();

        // 速度コンポーネントがない場合はスキップ
        if (!velocity) {
            continue;
        }

        // PositionComponentを取得（CartesianとPolarの両方を確認）
        auto positionCartesian = point.getComponent<PositionComponent<CoordinateSystem::Cartesian>>();
        if (positionCartesian) {
            // デカルト座標系の場合、直接速度を加算
            positionCartesian->position += velocity->velocity;
        } else {
            auto positionPolar = point.getComponent<PositionComponent<CoordinateSystem::Polar>>();
            if (positionPolar) {
                // 極座標系の場合、デカルト座標に変換
                glm::vec3 cartesianPos = positionPolar->getTransformedPosition();

                // 速度を加算
                cartesianPos += velocity->velocity;

                // 新しい位置を極座標系に変換し直す
                float r = glm::length(cartesianPos);
                float theta = atan2(cartesianPos.y, cartesianPos.x);
                float phi = acos(cartesianPos.z / r);
                positionPolar->position = glm::vec3(r, theta, phi);
            }
        }
    }
}

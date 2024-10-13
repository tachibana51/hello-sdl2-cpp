// src/generatePointCloud.cpp
#include "generatePointCloud.h"
#include "PositionComponent.h"
#include "VelocityComponent.h"
#include "CoordinateLabelComponent.h"
#include <glm/glm.hpp>
#include <cstdlib>

std::vector<Entity> generatePointCloud(int numPoints) {
    std::vector<Entity> pointCloud;

    for (int i = 0; i < numPoints; ++i) {
        Entity point;
        glm::vec3 position(
            static_cast<float>(rand()) / RAND_MAX * 10.0f,
            static_cast<float>(rand()) / RAND_MAX * 10.0f,
            static_cast<float>(rand()) / RAND_MAX * 10.0f
        );
        glm::vec3 velocity(
            static_cast<float>(rand()) / RAND_MAX * 0.1f,
            static_cast<float>(rand()) / RAND_MAX * 0.1f,
            static_cast<float>(rand()) / RAND_MAX * 0.1f
        );

        // テンプレート引数を指定してコンポーネントを追加
        point.addComponent<PositionComponent<CoordinateSystem::Cartesian>>(position);
        point.addComponent<VelocityComponent>(velocity);
        point.addComponent<CoordinateLabelComponent>("");

        pointCloud.push_back(point);
    }

    return pointCloud;
}

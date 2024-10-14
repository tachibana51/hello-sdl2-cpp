
#pragma once
#include <functional>
#include <glm/glm.hpp>

struct FunctionComponent {
    std::function<glm::vec3(float)> positionFunc;
    std::function<glm::vec3(float)> velocityFunc;
};

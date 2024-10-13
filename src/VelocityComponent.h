// VelocityComponent.h
#pragma once
#include <glm/glm.hpp>

struct VelocityComponent {
    glm::vec3 velocity;

    VelocityComponent(const glm::vec3& vel) : velocity(vel) {}
};

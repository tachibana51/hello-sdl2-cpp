// CameraComponent.h
#pragma once
#include <glm/glm.hpp>

struct CameraComponent {
    glm::vec3 position;
    glm::vec3 rotation;

    CameraComponent(const glm::vec3& pos, const glm::vec3& rot)
        : position(pos), rotation(rot) {}
};

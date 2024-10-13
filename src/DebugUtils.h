// DebugUtils.h
#ifndef DEBUGUTILS_H
#define DEBUGUTILS_H

#include <iostream>
#include <glm/glm.hpp>

// デバッグメッセージの出力
inline void logMessage(const std::string& message) {
    std::cout << "[DEBUG] " << message << std::endl;
}

// glm::vec3 の値をログ出力
inline void logVec3(const std::string& name, const glm::vec3& vec) {
    std::cout << "[DEBUG] " << name << ": (" << vec.x << ", " << vec.y << ", " << vec.z << ")" << std::endl;
}

#endif // DEBUGUTILS_H

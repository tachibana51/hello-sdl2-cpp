#pragma once

#include "../Engine/ECS/System.hpp"
#include "../Engine/ECS/Coordinator.hpp"
#include "../Components/CameraComponent.hpp"
#include "../Components/ProjectionComponent.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL.h>
#include <cmath>

class CameraSystem : public ECS::System {
public:
    CameraSystem()
        : coordinator(nullptr),
          windowWidth(800),
          windowHeight(600),
          dragging(false),
          lastMouseX(0),
          lastMouseY(0),
          cameraRadius(20.0f) // 初期半径
    {}
    ~CameraSystem() {}

    void setCoordinator(ECS::Coordinator* coord) {
        coordinator = coord;
    }

    void setWindowSize(int width, int height) {
        windowWidth = width;
        windowHeight = height;
    }

    // マウスイベントの処理
    void handleMouseEvent(const SDL_Event& event) {
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                dragging = true;
                lastMouseX = event.button.x;
                lastMouseY = event.button.y;
                SDL_Log("Mouse drag started at (%d, %d)", lastMouseX, lastMouseY);
            }
        }
        else if (event.type == SDL_MOUSEBUTTONUP) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                dragging = false;
                SDL_Log("Mouse drag ended");
            }
        }
        else if (event.type == SDL_MOUSEMOTION) {
            if (dragging) {
                int dx = event.motion.x - lastMouseX;
                int dy = event.motion.y - lastMouseY;
                lastMouseX = event.motion.x;
                lastMouseY = event.motion.y;

                float sensitivity = 0.005f;
                float deltaYaw = dx * sensitivity;
                float deltaPitch = dy * sensitivity;

                for (auto const& entity : entities) {
                    if (coordinator->hasComponent<CameraComponent>(entity)) {
                        auto& camera = coordinator->getComponent<CameraComponent>(entity);
                        camera.yaw += deltaYaw;
                        camera.pitch += deltaPitch;

                        // ピッチの制限（上下方向の回転を制限）
                        if (camera.pitch > glm::radians(89.0f)) camera.pitch = glm::radians(89.0f);
                        if (camera.pitch < glm::radians(-89.0f)) camera.pitch = glm::radians(-89.0f);

                        // カメラの位置を再計算（オービットカメラとして）
                        updateCameraPosition(camera);

                        SDL_Log("Camera updated: yaw=%.2f, pitch=%.2f, position=(%.2f, %.2f, %.2f)",
                                camera.yaw, camera.pitch, camera.position.x, camera.position.y, camera.position.z);
                    }
                }
            }
        }
        else if (event.type == SDL_MOUSEWHEEL) {
            // マウスホイールのスクロールによるカメラのズーム
            // event.wheel.y > 0: scroll up, event.wheel.y < 0: scroll down
            float zoomSensitivity = 1.0f; // 調整可能
            float deltaRadius = event.wheel.y * zoomSensitivity;
            float newRadius = cameraRadius - deltaRadius; // スクロールアップでズームイン（半径を減らす）

            // 最小・最大半径の設定
            const float minRadius = 5.0f;
            const float maxRadius = 100.0f;

            // クランプ
            if (newRadius < minRadius) newRadius = minRadius;
            if (newRadius > maxRadius) newRadius = maxRadius;

            if (newRadius != cameraRadius) {
                cameraRadius = newRadius;

                // カメラの位置を再計算（オービットカメラとして）
                for (auto const& entity : entities) {
                    if (coordinator->hasComponent<CameraComponent>(entity)) {
                        auto& camera = coordinator->getComponent<CameraComponent>(entity);
                        updateCameraPosition(camera);
                        SDL_Log("Camera radius updated: %.2f, new position=(%.2f, %.2f, %.2f)",
                                cameraRadius, camera.position.x, camera.position.y, camera.position.z);
                    }
                }
            }
        }
    }

    // カメラ行列の計算
    glm::mat4 getViewMatrix() const {
        for (auto const& entity : entities) {
            if (coordinator->hasComponent<CameraComponent>(entity)) {
                auto& camera = coordinator->getComponent<CameraComponent>(entity);
                // 常に原点を向く
                return glm::lookAt(camera.position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            }
        }
        return glm::mat4(1.0f);
    }

    glm::mat4 getProjectionMatrix() const {
        for (auto const& entity : entities) {
            if (coordinator->hasComponent<ProjectionComponent>(entity)) {
                auto& proj = coordinator->getComponent<ProjectionComponent>(entity);
                float aspect = proj.aspectRatio;
                float nearP = proj.nearPlane;
                float farP = proj.farPlane;

                // 透視投影行列
                return glm::perspective(glm::radians(45.0f), aspect, nearP, farP);
            }
        }
        return glm::mat4(1.0f);
    }

private:
    ECS::Coordinator* coordinator;
    int windowWidth;
    int windowHeight;
    bool dragging;
    int lastMouseX;
    int lastMouseY;
    float cameraRadius; // カメラと原点の距離

    // カメラの位置を更新する関数
    void updateCameraPosition(CameraComponent& camera) const {
        // 球面座標からデカルト座標への変換
        float x = cameraRadius * cos(camera.pitch) * cos(camera.yaw);
        float y = cameraRadius * sin(camera.pitch);
        float z = cameraRadius * cos(camera.pitch) * sin(camera.yaw);
        camera.position = glm::vec3(x, y, z);
    }
};

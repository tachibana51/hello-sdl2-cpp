
#pragma once

#include "../Engine/ECS/System.h"
#include "../Engine/ECS/Coordinator.h"
#include "../Components/PositionComponent.h"
#include "../Components/MorphingComponent.h"
#include "CameraSystem.h" // 完全な定義をインクルード
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class RenderSystem : public ECS::System {
public:
    RenderSystem() : coordinator(nullptr), renderer(nullptr), windowWidth(800), windowHeight(600), cameraSystem(nullptr) {}
    ~RenderSystem() {}

    void setCoordinator(ECS::Coordinator* coord) {
        coordinator = coord;
    }

    void setRenderer(SDL_Renderer* rend) {
        renderer = rend;
    }

    void setWindowSize(int width, int height) {
        windowWidth = width;
        windowHeight = height;
    }

    void setCameraSystem(CameraSystem* camSys) { // 修正
        cameraSystem = camSys;
    }

    void update(float deltaTime) { // 'override' を削除
        if (!renderer || !coordinator || !cameraSystem) {
            SDL_Log("RenderSystem: Missing renderer, coordinator, or cameraSystem.");
            return;
        }

        // カメラ行列と投影行列の取得
        glm::mat4 view = cameraSystem->getViewMatrix();
        glm::mat4 projection = cameraSystem->getProjectionMatrix();

        // 点群の描画
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 白
        int drawableEntities = 0; // 描画対象のエンティティ数
        int loggedPoints = 0; // ログ出力した点の数
        for (auto const& entity : entities) {
            if (!coordinator->hasComponent<PositionComponent>(entity))
                continue;

            drawableEntities++;

            auto& position = coordinator->getComponent<PositionComponent>(entity).position;

            // ワールド座標からクリップ空間へ
            glm::vec4 clipSpacePos = projection * view * glm::vec4(position, 1.0f);

            // NDC（Normalized Device Coordinates）に変換
            if (clipSpacePos.w == 0.0f) continue; // ゼロ除算を避ける
            glm::vec3 ndc = glm::vec3(clipSpacePos) / clipSpacePos.w;

            // スクリーン座標に変換
            int screenX = static_cast<int>((ndc.x + 1.0f) * 0.5f * windowWidth);
            int screenY = static_cast<int>((1.0f - ndc.y) * 0.5f * windowHeight); // Y軸反転

            // 視野外の点を描画しない
            if (ndc.x < -1.0f || ndc.x > 1.0f || ndc.y < -1.0f || ndc.y > 1.0f || ndc.z < -1.0f || ndc.z > 1.0f)
                continue;

            // スクリーン座標がウィンドウ内か確認
            if (screenX < 0 || screenX >= windowWidth || screenY < 0 || screenY >= windowHeight)
                continue;

            // 特定のエンティティID（例: 0）に対してスクリーン座標をログ出力
            if (entity == 0 && loggedPoints < 5) { // 最初の5点のみログ
                SDL_Log("Entity %d: Screen Position = (%d, %d)", entity, screenX, screenY);
                loggedPoints++;
            }

            // 小さな矩形としてポイントを描画
            SDL_Rect pointRect = { screenX, screenY, 2, 2 };
            if (SDL_RenderFillRect(renderer, &pointRect) != 0) {
                SDL_Log("SDL_RenderFillRect failed: %s", SDL_GetError());
            }
        }

        SDL_Log("RenderSystem: Drawable Entities = %d", drawableEntities);

        // 座標軸の描画
        drawAxes(view, projection);
    }

private:
    ECS::Coordinator* coordinator;
    SDL_Renderer* renderer;
    int windowWidth;
    int windowHeight;
    CameraSystem* cameraSystem; // CameraSystem のポインタ

    void drawAxes(const glm::mat4& view, const glm::mat4& projection) {
        // X軸（赤）、Y軸（緑）、Z軸（青）
        struct Axis {
            glm::vec3 start;
            glm::vec3 end;
            SDL_Color color;
        };

        std::vector<Axis> axes = {
            { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 0.0f, 0.0f), SDL_Color{255, 0, 0, 255} }, // X軸
            { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 10.0f, 0.0f), SDL_Color{0, 255, 0, 255} }, // Y軸
            { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 10.0f), SDL_Color{0, 0, 255, 255} }  // Z軸
        };

        for (auto& axis : axes) {
            // ワールド座標からクリップ空間へ
            glm::vec4 startClip = projection * view * glm::vec4(axis.start, 1.0f);
            glm::vec4 endClip = projection * view * glm::vec4(axis.end, 1.0f);

            // NDCに変換
            if (startClip.w == 0.0f || endClip.w == 0.0f) continue;
            glm::vec3 startNDC = glm::vec3(startClip) / startClip.w;
            glm::vec3 endNDC = glm::vec3(endClip) / endClip.w;

            // スクリーン座標に変換
            int startX = static_cast<int>((startNDC.x + 1.0f) * 0.5f * windowWidth);
            int startY = static_cast<int>((1.0f - startNDC.y) * 0.5f * windowHeight);
            int endX = static_cast<int>((endNDC.x + 1.0f) * 0.5f * windowWidth);
            int endY = static_cast<int>((1.0f - endNDC.y) * 0.5f * windowHeight);

            // 線を描画
            SDL_SetRenderDrawColor(renderer, axis.color.r, axis.color.g, axis.color.b, axis.color.a);
            if (SDL_RenderDrawLine(renderer, startX, startY, endX, endY) != 0) {
                SDL_Log("SDL_RenderDrawLine failed: %s", SDL_GetError());
            }
        }
    }
};

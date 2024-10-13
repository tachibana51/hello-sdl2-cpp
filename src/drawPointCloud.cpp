// src/drawPointCloud.cpp
#include "drawPointCloud.h"
#include "PositionComponent.h"
#include "CoordinateLabelComponent.h"
#include "DebugUtils.h"
#include <SDL2/SDL.h>

glm::vec3 projectTo2D(const glm::vec3& point, const glm::vec3& cameraPosition) {
    float z = point.z - cameraPosition.z + 1.0f;  // 簡易的なカメラのzオフセット処理
    if (z == 0.0f) z = 0.1f;  // z=0で無限大になるのを防ぐ

    // 投影変換：3D座標を2Dスクリーン座標に変換
    int screenX = static_cast<int>((point.x - cameraPosition.x) / z * 300.0f) + 400;
    int screenY = static_cast<int>((point.y - cameraPosition.y) / z * 300.0f) + 300;

    return glm::vec3(screenX, screenY, 0.0f);
}

void drawPointCloud(const std::vector<Entity>& pointCloud, SDL_Renderer* renderer,
int windowWidth,
int windowHeight
) {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (const auto& point : pointCloud) {

        // テンプレート引数を指定してコンポーネントを取得
        auto positionCartesian = point.getComponent<PositionComponent<CoordinateSystem::Cartesian>>();
        auto positionPolar = point.getComponent<PositionComponent<CoordinateSystem::Polar>>();
        auto label = point.getComponent<CoordinateLabelComponent>();
        glm::vec3 pos;

        if (positionCartesian) {
            pos = positionCartesian->getTransformedPosition();
            logVec3("posCartesian",pos);
        } else if (positionPolar) {
            pos = positionPolar->getTransformedPosition();
            logVec3("posPolar",pos);
        } else {
            continue; // PositionComponentがない場合はスキップ
        }

        // スケーリングを調整
        float scaleX = static_cast<float>(windowWidth) / 10.0f; // 基準値10.0f
        float scaleY = static_cast<float>(windowHeight) / 10.0f;

        int screenX = static_cast<int>(pos.x * scaleX);
        int screenY = static_cast<int>(pos.y * scaleY);
        glm::vec3 screenPos = projectTo2D(pos, glm::vec3(0.0f, 0.0f, 5.0f));  // カメラ位置は仮に原点の上方5ユニット
        // Y座標を反転（SDLの座標系に合わせる）
        screenY = windowHeight - screenY;
        if (label) {
            // 点を描画
            SDL_RenderDrawPoint(renderer, screenX, screenY);

            // 座標ラベルを描画（省略）
        }
    }
}

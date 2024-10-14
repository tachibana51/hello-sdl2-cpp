// src/Systems/WaveletVisualizationSystem.h
#pragma once

#include "../Engine/ECS/System.h"
#include "../Engine/ECS/Coordinator.h"
#include "../Components/WaveletComponent.h"
#include "../Components/WaveletVisualizationComponent.h"
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <vector>
#include <cmath>

class WaveletVisualizationSystem : public ECS::System {
public:
    WaveletVisualizationSystem() : coordinator(nullptr), timePerUnit(0.01f) {}
    ~WaveletVisualizationSystem() {}

    void setCoordinator(ECS::Coordinator* coord) {
        coordinator = coord;
    }

    // ウェーブレット係数を3D点群にマッピング
    void visualizeWavelet(ECS::Entity entity) {
        if (!coordinator) {
            SDL_Log("WaveletVisualizationSystem: Coordinator is null.");
            return;
        }

        if (!coordinator->hasComponent<WaveletComponent>(entity)) {
            SDL_Log("WaveletVisualizationSystem: Entity %d does not have WaveletComponent.", entity);
            return;
        }

        const auto& wavelet = coordinator->getComponent<WaveletComponent>(entity);

        std::vector<glm::vec3> points;
        std::vector<glm::vec4> colors;

        // 基本的なマッピング方法:
        // - x: 時間
        // - y: 周波数（スケール）
        // - z: 振幅

        for (int s = 0; s < wavelet.scaleCount; ++s) {
            const auto& detail = wavelet.detailCoefficients[s];
            int frequency = s + 1; // スケールに基づく周波数の仮定

            for (size_t i = 0; i < detail.size(); ++i) {
                float time = static_cast<float>(i) * timePerUnit;
                float amplitude = std::abs(detail[i]);

                // 座標軸のスケーリング
                float x = time;
                float y = static_cast<float>(frequency);
                float z = amplitude;

                points.emplace_back(glm::vec3(x, y, z));

                // 振幅に基づいて色を決定（大きいほど明るい色）
                float intensity = glm::clamp(amplitude / 1.0f, 0.0f, 1.0f);
                glm::vec4 color = glm::vec4(intensity, 0.0f, 1.0f - intensity, 1.0f); // 青から赤へのグラデーション
                colors.emplace_back(color);
            }
        }

        // WaveletVisualizationComponent をエンティティに追加
        coordinator->addComponent<WaveletVisualizationComponent>(entity, WaveletVisualizationComponent{
            std::move(points),
            std::move(colors)
        });

        SDL_Log("Wavelet visualization performed on entity %d.", entity);
    }

    void update(float deltaTime) {
        // 現在の実装では自動的に可視化を行わない
        // 必要に応じて、エンティティの状態に応じて処理を行うように拡張可能
    }

private:
    ECS::Coordinator* coordinator;
    float timePerUnit; // 時間軸のスケール（調整可能）
};

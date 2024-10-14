// src/Systems/WaveletSystem.h
#pragma once

#include "../Engine/ECS/System.h"
#include "../Engine/ECS/Coordinator.h"
#include "../Components/AudioComponent.h"
#include "../Components/WaveletComponent.h"
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <vector>
#include <cmath>

class WaveletSystem : public ECS::System {
public:
    WaveletSystem() : coordinator(nullptr), maxScale(5) {}
    ~WaveletSystem() {}

    void setCoordinator(ECS::Coordinator* coord) {
        coordinator = coord;
    }

    // シンプルな Haar ウェーブレット変換
    void performWaveletTransform(ECS::Entity entity) {
        if (!coordinator) {
            SDL_Log("WaveletSystem: Coordinator is null.");
            return;
        }

        if (!coordinator->hasComponent<AudioComponent>(entity)) {
            SDL_Log("WaveletSystem: Entity %d does not have AudioComponent.", entity);
            return;
        }

        const auto& audio = coordinator->getComponent<AudioComponent>(entity);
        std::vector<float> data = audio.samples; // サンプルデータをコピー

        int originalLength = data.size();
        int scale = maxScale;

        // ウェーブレット変換係数の格納
        std::vector<std::vector<float>> coefficients;

        for (int s = 0; s < scale; ++s) {
            int currentLength = data.size();
            if (currentLength < 2) break;

            std::vector<float> approx;
            std::vector<float> detail;

            for (size_t i = 0; i + 1 < data.size(); i += 2) {
                float avg = (data[i] + data[i + 1]) / 2.0f;
                float diff = (data[i] - data[i + 1]) / 2.0f;
                approx.push_back(avg);
                detail.push_back(diff);
            }

            coefficients.push_back(detail); // 詳細係数を保存

            data = approx; // 近似係数を次のスケールの入力に
        }

        // WaveletComponent をエンティティに追加
        coordinator->addComponent<WaveletComponent>(entity, WaveletComponent{
            std::move(coefficients),
            maxScale
        });

        SDL_Log("Wavelet transform performed on entity %d with %d scales.", entity, maxScale);
    }

    // シンプルな逆 Haar ウェーブレット変換
    void performInverseWaveletTransform(ECS::Entity entity) {
        if (!coordinator) {
            SDL_Log("WaveletSystem: Coordinator is null.");
            return;
        }

        if (!coordinator->hasComponent<WaveletComponent>(entity)) {
            SDL_Log("WaveletSystem: Entity %d does not have WaveletComponent.", entity);
            return;
        }

        const auto& wavelet = coordinator->getComponent<WaveletComponent>(entity);

        std::vector<float> data;

        // 逆ウェーブレット変換
        for (int s = wavelet.scaleCount - 1; s >= 0; --s) {
            const auto& detail = wavelet.coefficients[s];
            std::vector<float> approx;

            // 近似係数の初期化（スケールごとに適宜調整）
            if (data.empty()) {
                // 最後のスケールの近似係数を仮定
                // 実際には WaveletComponent に近似係数も保持する必要があります
                // ここではシンプルな例として詳細係数のみを使用
                continue;
            }

            // 逆変換処理（ここでは詳細係数のみを使用して再構成）
            // 実際の実装では、近似係数も保持している必要があります

            // 省略: 詳細係数と近似係数を組み合わせて再構成
        }

        // AudioComponent のサンプルデータを更新
        // 実装には近似係数も保持する必要があるため、ここでは簡略化
        // 実際の用途に合わせて適切に実装してください
    }

    void update(float deltaTime)  {
        // 現在の実装では自動的にウェーブレット変換を行わない
        // 必要に応じて、エンティティの状態に応じて処理を行うように拡張可能
    }

private:
    ECS::Coordinator* coordinator;
    int maxScale; // ウェーブレット変換の最大スケール
};

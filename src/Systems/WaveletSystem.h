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

        int scale = maxScale;

        // ウェーブレット変換係数の格納
        std::vector<std::vector<float>> detailCoefficients;
        std::vector<std::vector<float>> approxCoefficients;

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

            approxCoefficients.push_back(approx);
            detailCoefficients.push_back(detail);

            data = approx; // 近似係数を次のスケールの入力に
        }

        // WaveletComponent をエンティティに追加
        coordinator->addComponent<WaveletComponent>(entity, WaveletComponent{
            std::move(detailCoefficients),
            std::move(approxCoefficients),
            scale
        });

        SDL_Log("Wavelet transform performed on entity %d with %d scales.", entity, scale);
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

        // 逆変換のために近似係数と詳細係数を使用
        std::vector<float> approx = wavelet.approxCoefficients[wavelet.scaleCount - 1];
        for (int s = wavelet.scaleCount - 1; s >= 0; --s) {
            const auto& detail = wavelet.detailCoefficients[s];
            std::vector<float> reconstructed;

            for (size_t i = 0; i < approx.size(); ++i) {
                float avg = approx[i];
                float diff = detail[i];
                float a = avg + diff;
                float b = avg - diff;
                reconstructed.push_back(a);
                reconstructed.push_back(b);
            }

            approx = reconstructed;
        }

        // AudioComponent のサンプルデータを更新
        auto& audio = coordinator->getComponent<AudioComponent>(entity);
        audio.samples = approx;

        SDL_Log("Inverse wavelet transform performed on entity %d.", entity);
    }

    void update(float deltaTime)  {
        // 現在の実装では自動的にウェーブレット変換を行わない
    }

private:
    ECS::Coordinator* coordinator;
    int maxScale; // ウェーブレット変換の最大スケール
};

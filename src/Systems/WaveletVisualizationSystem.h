
// src/Systems/WaveletVisualizationSystem.h
#pragma once

#include "../Engine/ECS/System.h"
#include "../Engine/ECS/Coordinator.h"
#include "../Components/WaveletVisualizationComponent.h"
#include "CameraSystem.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

class WaveletVisualizationSystem : public ECS::System {
public:
    WaveletVisualizationSystem()
        : coordinator(nullptr),
          renderer(nullptr),
          windowWidth(800),
          windowHeight(600),
          cameraSystem(nullptr),
          font(nullptr),
          amplitudeThreshold(0.5f) // 初期値
    {}

    ~WaveletVisualizationSystem() {
        if (font) {
            TTF_CloseFont(font);
            font = nullptr;
        }
    }

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

    void setCameraSystem(CameraSystem* camSys) {
        cameraSystem = camSys;
    }

    // フォントをロードするメソッドを追加
    bool loadFont(const std::string& fontPath, int fontSize) {
        font = TTF_OpenFont(fontPath.c_str(), fontSize);
        if (!font) {
            SDL_Log("Failed to load font %s: %s", fontPath.c_str(), TTF_GetError());
            return false;
        }
        return true;
    }

    // 振幅閾値を設定するメソッド
    void setAmplitudeThreshold(float threshold) {
        amplitudeThreshold = threshold;
    }

    void update(float deltaTime) {
        if (!renderer || !coordinator || !cameraSystem) {
            SDL_Log("WaveletVisualizationSystem: Missing renderer, coordinator, or cameraSystem.");
            return;
        }

        if (!font) {
            SDL_Log("WaveletVisualizationSystem: Font not loaded.");
            return;
        }

        // カメラ行列と投影行列の取得
        glm::mat4 view = cameraSystem->getViewMatrix();
        glm::mat4 projection = cameraSystem->getProjectionMatrix();

        // WaveletVisualizationComponent を持つエンティティの描画
        for (auto const& entity : entities) {
            const auto& waveletVis = coordinator->getComponent<WaveletVisualizationComponent>(entity);
            for (size_t i = 0; i < waveletVis.points.size(); ++i) {
                const glm::vec3& point = waveletVis.points[i];
                const glm::vec4& color = waveletVis.colors[i];

                // 振幅閾値を超えているか確認
                if (point.z < amplitudeThreshold)
                    continue;

                // ワールド座標からクリップ空間へ
                glm::vec4 clipSpacePos = projection * view * glm::vec4(point, 1.0f);

                // NDCに変換
                if (clipSpacePos.w == 0.0f) continue;
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

                // 色の設定
                SDL_SetRenderDrawColor(renderer,
                                       static_cast<Uint8>(color.r * 255),
                                       static_cast<Uint8>(color.g * 255),
                                       static_cast<Uint8>(color.b * 255),
                                       static_cast<Uint8>(color.a * 255));

                // 点を描画（円形にするために複数の点を描画）
                const int radius = 2;
                drawCircle(screenX, screenY, radius);

                // 座標ラベルの描画
                renderCoordinateLabel(point, screenX, screenY);
            }
        }

        SDL_Log("WaveletVisualizationSystem: Render complete.");
    }

private:
    ECS::Coordinator* coordinator;
    SDL_Renderer* renderer;
    int windowWidth;
    int windowHeight;
    CameraSystem* cameraSystem;
    TTF_Font* font; // フォント
    float amplitudeThreshold; // 座標ラベル表示の閾値

    // 座標ラベルを描画する関数
    void renderCoordinateLabel(const glm::vec3& position, int screenX, int screenY) {
        if (!font) return;

        // テキスト内容を作成
        std::string text = "(" + std::to_string(position.x) + ", " +
                            std::to_string(position.y) + ", " +
                            std::to_string(position.z) + ")";

        // テキストをサーフェスにレンダリング
        SDL_Color textColor = { 255, 255, 255, 255 }; // 白色
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
        if (!textSurface) {
            SDL_Log("Failed to render text surface: %s", TTF_GetError());
            return;
        }

        // サーフェスをテクスチャに変換
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (!textTexture) {
            SDL_Log("Failed to create text texture: %s", SDL_GetError());
            SDL_FreeSurface(textSurface);
            return;
        }

        // テクスチャのサイズを取得
        int textWidth = textSurface->w;
        int textHeight = textSurface->h;

        // テクスチャの描画位置を設定（点の右下に表示）
        SDL_Rect renderQuad = { screenX + 5, screenY - textHeight - 5, textWidth, textHeight };

        // テクスチャをレンダラーにコピー
        if (SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad) != 0) {
            SDL_Log("Failed to render text texture: %s", SDL_GetError());
        }

        // リソースの解放
        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);
    }

    // 点を円形に描画する補助関数
    void drawCircle(int x, int y, int radius) {
        for (int w = 0; w < radius * 2; w++) {
            for (int h = 0; h < radius * 2; h++) {
                int dx = radius - w; // 左からの距離
                int dy = radius - h; // 上からの距離
                if ((dx*dx + dy*dy) <= (radius * radius)) {
                    SDL_RenderDrawPoint(renderer, x + dx, y + dy);
                }
            }
        }
    }
};

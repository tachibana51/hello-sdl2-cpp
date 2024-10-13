// src/main.cpp
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "Entity.h"
#include "generatePointCloud.h"
#include "updatePositions.h"
#include "renderScene.h"
#include "handleCameraMovement.h"
#include "onButtonPress.h"
#include "onMouseMovement.h"
#include "CameraComponent.h"
#include "DebugRenderUtils.h"
#include "DebugUtils.h"
int main(int argc, char* argv[]) {
    // SDLの初期化
    SDL_Init(SDL_INIT_VIDEO);

    const int INITIAL_WINDOW_WIDTH = 800;
    const int INITIAL_WINDOW_HEIGHT = 600;

    int windowWidth = INITIAL_WINDOW_WIDTH;
    int windowHeight = INITIAL_WINDOW_HEIGHT;

    SDL_Window* window = SDL_CreateWindow("Point Cloud Visualization",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          windowWidth, windowHeight,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

   initFont();
   TTF_Font* font = TTF_OpenFont("./JetBrainsMonoNL-Regular.ttf", 24);
   if(!font){
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return -1;
   }
    // 点群とカメラの初期化
    std::vector<Entity> pointCloud = generatePointCloud(1000);
    Entity camera;
    camera.addComponent<CameraComponent>(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f));

    bool running = true;
    SDL_Event event;

    while (running) {
        // イベント処理
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    windowWidth = event.window.data1;
                    windowHeight = event.window.data2;
                }
            }
            // ボタン押下の処理（仮想的なボタンイベントとして処理）
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_g) {
                  logMessage("generate");
                    onButtonPress(pointCloud, "generate");
                } else if (event.key.keysym.sym == SDLK_c) {
                    onButtonPress(pointCloud, "switch");
                    onButtonPress(pointCloud, "switchCoordinate");
                }
                handleCameraMovement(camera, event);
            }
            // マウス移動の処理
            if (event.type == SDL_MOUSEMOTION) {
                onMouseMovement(camera, event);
            }
        }

        // 点群の位置更新
        updatePositions(pointCloud);

        // シーンのレンダリング
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        renderScene(pointCloud, camera, renderer, windowWidth, windowHeight);
         // テキスト描画
        SDL_Color white = { 255, 255, 255, 255 };
        renderText(renderer, "FPS: 60", 10, 10, font, white);
        SDL_RenderPresent(renderer);
    }

    // SDLのクリーンアップ
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

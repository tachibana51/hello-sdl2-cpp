// renderScene.cpp
#include "renderScene.h"
#include <vector>
#include "Entity.h"
#include "drawPointCloud.h"
#include <SDL2/SDL.h>

void renderScene(const std::vector<Entity>& pointCloud, Entity& camera, SDL_Renderer* renderer, int windowWidth, int windowHeight) {
    // カメラの視点を考慮して点群を描画（簡易的な実装）
    drawPointCloud(pointCloud, renderer, windowWidth, windowHeight);

}

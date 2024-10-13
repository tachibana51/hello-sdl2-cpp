// renderScene.h
#pragma once
#include <vector>
#include "Entity.h"
#include "drawPointCloud.h"
#include <SDL2/SDL.h>

void renderScene(const std::vector<Entity>& pointCloud, Entity& camera, SDL_Renderer* renderer,int windowWidth, int windowHeight);

// drawPointCloud.h
#pragma once
#include <vector>
#include "Entity.h"
#include "PositionComponent.h"
#include "CoordinateLabelComponent.h"
#include <SDL2/SDL.h>

void drawPointCloud(const std::vector<Entity>& pointCloud, SDL_Renderer* renderer,int windowWidth,int windowHeight);

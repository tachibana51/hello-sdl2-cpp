// handleCameraMovement.h
#pragma once
#include "Entity.h"
#include "CameraComponent.h"
#include <SDL2/SDL.h>

void handleCameraMovement(Entity& camera, const SDL_Event& event);

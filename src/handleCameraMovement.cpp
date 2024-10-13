// handleCameraMovement.cpp
#include "handleCameraMovement.h"
#include "Entity.h"
#include "CameraComponent.h"
#include <SDL2/SDL.h>

void handleCameraMovement(Entity& camera, const SDL_Event& event) {
    auto cameraComp = camera.getComponent<CameraComponent>();
    if (cameraComp) {
        // キー入力に応じたカメラ移動の処理（例）
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_w:
                    cameraComp->position.y -= 1.0f;
                    break;
                case SDLK_s:
                    cameraComp->position.y += 1.0f;
                    break;
                case SDLK_a:
                    cameraComp->position.x -= 1.0f;
                    break;
                case SDLK_d:
                    cameraComp->position.x += 1.0f;
                    break;
                // その他のキー処理
            }
        }
    }
}

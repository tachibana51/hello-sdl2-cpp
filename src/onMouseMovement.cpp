// onMouseMovement.h
#include "onMouseMovement.h"
#include "Entity.h"
#include "CameraComponent.h"
#include <SDL2/SDL.h>

void onMouseMovement(Entity& camera, const SDL_Event& event) {
    auto cameraComp = camera.getComponent<CameraComponent>();
    if (cameraComp) {
        // マウス入力に応じたカメラ視点の移動処理（省略）
        // 例えば、マウスの移動量に応じてカメラの回転を更新
    }
}

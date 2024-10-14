
#pragma once
#include <SDL2/SDL_log.h>
#include "../Engine/ECS/System.h"
#include "../Engine/ECS/Coordinator.h"
#include "../Components/PositionComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/FunctionComponent.h"
#include "../Components/MorphingComponent.h"

class MovementSystem : public ECS::System {
public:
// Systems/MorphingSystem.h 内の update メソッドにデバッグログを追加
  void update(float deltaTime) {
      for (auto const& entity : entities) {
        if (!coordinator->hasComponent<MorphingComponent>(entity))
            continue;

        auto& morph = coordinator->getComponent<MorphingComponent>(entity);
        morph.elapsed += deltaTime;

        float t = morph.elapsed / morph.duration;
        if (t >= 1.0f) {
            t = 1.0f;
        }

        // 線形補間で位置を更新
        glm::vec3 newPosition = glm::mix(morph.startPosition, morph.targetPosition, t);
        coordinator->getComponent<PositionComponent>(entity).position = newPosition;

        // デバッグログ
        SDL_Log("Entity %d: Morphing t=%.2f, New Position=(%.2f, %.2f, %.2f)",
                entity, t, newPosition.x, newPosition.y, newPosition.z);

        if (t >= 1.0f) {
            // Morphing 完了
            coordinator->removeComponent<MorphingComponent>(entity);
            SDL_Log("Entity %d: Morphing complete.", entity);
        }
      }
    }

    void setCoordinator(ECS::Coordinator* coord) {
        coordinator = coord;
    }

private:
    float currentTime = 0.0f;
    ECS::Coordinator* coordinator;
};

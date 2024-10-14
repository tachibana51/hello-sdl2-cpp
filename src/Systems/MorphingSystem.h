
#pragma once

#include "../Engine/ECS/System.h"
#include "../Engine/ECS/Coordinator.h"
#include "../Components/MorphingComponent.h"
#include "../Components/PositionComponent.h"
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

class MorphingSystem : public ECS::System {
public:
    MorphingSystem() : coordinator(nullptr) {}
    ~MorphingSystem() {}

    void setCoordinator(ECS::Coordinator* coord) {
        coordinator = coord;
    }

    void update(float deltaTime)  {
        if (!coordinator) {
            SDL_Log("MorphingSystem: Coordinator is null.");
            return;
        }

        for (auto const& entity : entities) {
            if (coordinator->hasComponent<MorphingComponent>(entity) &&
                coordinator->hasComponent<PositionComponent>(entity)) {

                auto& morph = coordinator->getComponent<MorphingComponent>(entity);
                auto& position = coordinator->getComponent<PositionComponent>(entity).position;

                morph.elapsed += deltaTime;
                float t = morph.elapsed / morph.duration;
                if (t > 1.0f) t = 1.0f;

                // 線形補間
                position = glm::mix(morph.startPosition, morph.targetPosition, t);

                SDL_Log("Entity %d: Morphing t=%.2f, Position=(%.2f, %.2f, %.2f)",
                        entity, t, position.x, position.y, position.z);

                if (t >= 1.0f) {
                    // モーフィング完了後、MorphingComponent を削除
                    coordinator->removeComponent<MorphingComponent>(entity);
                    SDL_Log("Morphing complete for entity %d.", entity);
                }
            }
        }
    }

private:
    ECS::Coordinator* coordinator;
};

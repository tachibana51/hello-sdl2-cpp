#pragma once

#include "../Engine/ECS/System.hpp"
#include "../Engine/ECS/Coordinator.hpp"
#include "../Components/MorphingComponent.hpp"
#include "../Components/PositionComponent.hpp"
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <vector>

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

        std::vector<ECS::Entity> completedEntities;

        // 反復処理中にコレクションを変更しない
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
                    // モーフィングが完了したエンティティをリストに追加
                    completedEntities.push_back(entity);
                }
            }
        }

        // 反復処理後に MorphingComponent を削除
        for (auto const& entity : completedEntities) {
            coordinator->removeComponent<MorphingComponent>(entity);
            SDL_Log("Morphing complete for entity %d.", entity);
        }
    }

private:
    ECS::Coordinator* coordinator;
};

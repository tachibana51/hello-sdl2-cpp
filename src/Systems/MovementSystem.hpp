// src/Systems/MovementSystem.hpp
#pragma once

#include "../Engine/ECS/System.hpp"
#include "../Engine/ECS/Coordinator.hpp"
#include "../Components/PositionComponent.hpp"
#include "../Components/VelocityComponent.hpp"
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <vector> // 追加: std::vector を使用するため

class MovementSystem : public ECS::System {
public:
    MovementSystem() : coordinator(nullptr) {}
    ~MovementSystem() {}

    void setCoordinator(ECS::Coordinator* coord) {
        coordinator = coord;
    }

    void update(float deltaTime) {
        if (!coordinator) {
            SDL_Log("MovementSystem: Coordinator is null.");
            return;
        }

        // 削除対象のエンティティを収集するリスト
        std::vector<ECS::Entity> toDelete;

        for (auto const& entity : entities) {
            if (coordinator->hasComponent<PositionComponent>(entity) &&
                coordinator->hasComponent<VelocityComponent>(entity)) {

                auto& position = coordinator->getComponent<PositionComponent>(entity).position;
                auto& velocity = coordinator->getComponent<VelocityComponent>(entity).velocity;

                // 位置を更新
                position += velocity * deltaTime;

                // 領域外に出たか確認（例: ±10ユニットの立方体）
                float boundary = 10.0f;
                if (position.x < -boundary || position.x > boundary ||
                    position.y < -boundary || position.y > boundary ||
                    position.z < -boundary || position.z > boundary) {
                    toDelete.push_back(entity);
                }
            }
        }

        // 削除対象のエンティティを削除
        for (auto const& entity : toDelete) {
            coordinator->destroyEntity(entity);
            SDL_Log("Entity %d deleted for moving out of bounds.", entity);
        }
    }

private:
    ECS::Coordinator* coordinator;
};

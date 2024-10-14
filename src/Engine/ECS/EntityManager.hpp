
#pragma once

#include "Types.hpp"
#include <queue>
#include <array>
#include <stdexcept>

namespace ECS {
    class EntityManager {
    public:
        EntityManager() {
            for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
                availableEntities.push(entity);
            }
        }

        Entity createEntity() {
            if (livingEntityCount >= MAX_ENTITIES) {
                throw std::runtime_error("Too many entities in existence.");
            }
            Entity id = availableEntities.front();
            availableEntities.pop();
            ++livingEntityCount;
            return id;
        }

        void destroyEntity(Entity entity) {
            signatures[entity].reset();
            availableEntities.push(entity);
            --livingEntityCount;
        }

        void setSignature(Entity entity, Signature signature) {
            signatures[entity] = signature;
        }

        Signature getSignature(Entity entity) {
            return signatures[entity];
        }

    private:
        std::queue<Entity> availableEntities{};
        std::array<Signature, MAX_ENTITIES> signatures{};
        std::uint32_t livingEntityCount{};
    };
}

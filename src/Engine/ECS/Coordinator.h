
#pragma once

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

namespace ECS {
    class Coordinator {
    public:
        void init() {
            entityManager = std::make_unique<EntityManager>();
            componentManager = std::make_unique<ComponentManager>();
            systemManager = std::make_unique<SystemManager>();
        }

        // Entity methods
        Entity createEntity() {
            return entityManager->createEntity();
        }

        void destroyEntity(Entity entity) {
            entityManager->destroyEntity(entity);
            componentManager->entityDestroyed(entity);
            systemManager->entityDestroyed(entity);
        }

        // Component methods
        template<typename T>
        void registerComponent() {
            componentManager->registerComponent<T>();
        }

        template<typename T>
        void addComponent(Entity entity, T component) {
            componentManager->addComponent<T>(entity, component);

            auto signature = entityManager->getSignature(entity);
            signature.set(componentManager->getComponentType<T>(), true);
            entityManager->setSignature(entity, signature);

            systemManager->entitySignatureChanged(entity, signature);
        }

        template<typename T>
        void removeComponent(Entity entity) {
            componentManager->removeComponent<T>(entity);

            auto signature = entityManager->getSignature(entity);
            signature.set(componentManager->getComponentType<T>(), false);
            entityManager->setSignature(entity, signature);

            systemManager->entitySignatureChanged(entity, signature);
        }

        template<typename T>
        T& getComponent(Entity entity) {
            return componentManager->getComponent<T>(entity);
        }

        template<typename T>
        ComponentType getComponentType() {
            return componentManager->getComponentType<T>();
        }

        template<typename T>
        bool hasComponent(Entity entity) {
            return componentManager->hasComponent<T>(entity);
        }

        // System methods
        template<typename T>
        std::shared_ptr<T> registerSystem() {
            return systemManager->registerSystem<T>();
        }

        template<typename T>
        void setSystemSignature(Signature signature) {
            systemManager->setSignature<T>(signature);
        }

    private:
        std::unique_ptr<EntityManager> entityManager;
        std::unique_ptr<ComponentManager> componentManager;
        std::unique_ptr<SystemManager> systemManager;
    };
}

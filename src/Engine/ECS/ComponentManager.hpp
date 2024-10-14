
#pragma once

#include "Types.hpp"
#include <any>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <array>
#include <stdexcept>

namespace ECS {
    class IComponentArray {
    public:
        virtual ~IComponentArray() = default;
        virtual void entityDestroyed(Entity entity) = 0;
        virtual bool hasData(Entity entity) const = 0;
    };

    template<typename T>
    class ComponentArray : public IComponentArray {
    public:
        void insertData(Entity entity, T component) {
            entityToIndexMap[entity] = size;
            indexToEntityMap[size] = entity;
            componentArray[size] = component;
            ++size;
        }

        void removeData(Entity entity) {
            size_t indexOfRemovedEntity = entityToIndexMap[entity];
            size_t indexOfLastElement = size - 1;
            componentArray[indexOfRemovedEntity] = componentArray[indexOfLastElement];

            Entity lastEntity = indexToEntityMap[indexOfLastElement];
            entityToIndexMap[lastEntity] = indexOfRemovedEntity;
            indexToEntityMap[indexOfRemovedEntity] = lastEntity;

            entityToIndexMap.erase(entity);
            indexToEntityMap.erase(indexOfLastElement);

            --size;
        }

        T& getData(Entity entity) {
            return componentArray[entityToIndexMap[entity]];
        }

        bool hasData(Entity entity) const override {
            return entityToIndexMap.find(entity) != entityToIndexMap.end();
        }

        void entityDestroyed(Entity entity) override {
            if (entityToIndexMap.find(entity) != entityToIndexMap.end()) {
                removeData(entity);
            }
        }

    private:
        std::array<T, MAX_ENTITIES> componentArray{};
        std::unordered_map<Entity, size_t> entityToIndexMap{};
        std::unordered_map<size_t, Entity> indexToEntityMap{};
        size_t size{};
    };

    class ComponentManager {
    public:
        template<typename T>
        void registerComponent() {
            const char* typeName = typeid(T).name();

            if (componentTypes.find(typeName) != componentTypes.end()) {
                throw std::runtime_error("Registering component type more than once.");
            }

            componentTypes.insert({typeName, nextComponentType});
            componentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

            ++nextComponentType;
        }

        template<typename T>
        ComponentType getComponentType() {
            const char* typeName = typeid(T).name();

            if (componentTypes.find(typeName) == componentTypes.end()) {
                throw std::runtime_error("Component not registered before use.");
            }

            return componentTypes[typeName];
        }

        template<typename T>
        void addComponent(Entity entity, T component) {
            getComponentArray<T>()->insertData(entity, component);
        }

        template<typename T>
        void removeComponent(Entity entity) {
            getComponentArray<T>()->removeData(entity);
        }

        template<typename T>
        T& getComponent(Entity entity) {
            return getComponentArray<T>()->getData(entity);
        }

        template<typename T>
        bool hasComponent(Entity entity) {
            return getComponentArray<T>()->hasData(entity);
        }

        void entityDestroyed(Entity entity) {
            for (auto const& pair : componentArrays) {
                auto const& component = pair.second;
                component->entityDestroyed(entity);
            }
        }

    private:
        std::unordered_map<const char*, ComponentType> componentTypes{};
        std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays{};
        ComponentType nextComponentType{};

        template<typename T>
        std::shared_ptr<ComponentArray<T>> getComponentArray() {
            const char* typeName = typeid(T).name();

            if (componentTypes.find(typeName) == componentTypes.end()) {
                throw std::runtime_error("Component not registered before use.");
            }

            return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeName]);
        }
    };
}

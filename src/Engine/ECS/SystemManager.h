
#pragma once

#include "System.h"
#include "Types.h"
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <stdexcept>

namespace ECS {
    class SystemManager {
    public:
        template<typename T>
        std::shared_ptr<T> registerSystem() {
            const char* typeName = typeid(T).name();

            if (systems.find(typeName) != systems.end()) {
                throw std::runtime_error("Registering system more than once.");
            }

            auto system = std::make_shared<T>();
            systems.insert({typeName, system});
            return system;
        }

        template<typename T>
        void setSignature(Signature signature) {
            const char* typeName = typeid(T).name();

            if (systems.find(typeName) == systems.end()) {
                throw std::runtime_error("System used before registered.");
            }

            signatures.insert({typeName, signature});
        }

        void entityDestroyed(Entity entity) {
            for (auto const& pair : systems) {
                auto const& system = pair.second;
                system->entities.erase(entity);
            }
        }

        void entitySignatureChanged(Entity entity, Signature entitySignature) {
            for (auto const& pair : systems) {
                auto const& typeName = pair.first;
                auto const& system = pair.second;
                auto const& systemSignature = signatures[typeName];

                if ((entitySignature & systemSignature) == systemSignature) {
                    system->entities.insert(entity);
                } else {
                    system->entities.erase(entity);
                }
            }
        }

    private:
        std::unordered_map<const char*, Signature> signatures{};
        std::unordered_map<const char*, std::shared_ptr<System>> systems{};
    };
}

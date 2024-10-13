// Entity.h
#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>

class Entity {
public:
    template <typename T, typename... Args>
    void addComponent(Args&&... args) {
        components[std::type_index(typeid(T))] = std::make_shared<T>(std::forward<Args>(args)...);
    }

    // 非const版
    template <typename T>
    std::shared_ptr<T> getComponent() {
        auto it = components.find(std::type_index(typeid(T)));
        if (it != components.end()) {
            return std::static_pointer_cast<T>(it->second);
        }
        return nullptr;
    }

    // const版
    template <typename T>
    std::shared_ptr<const T> getComponent() const {
        auto it = components.find(std::type_index(typeid(T)));
        if (it != components.end()) {
            return std::static_pointer_cast<const T>(it->second);
        }
        return nullptr;
    }

    // コンポーネントの削除
    template <typename T>
    void removeComponent() {
        components.erase(std::type_index(typeid(T)));
    }

private:
    std::unordered_map<std::type_index, std::shared_ptr<void>> components;
};

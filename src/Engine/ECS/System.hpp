
#pragma once
#include "Types.hpp"
#include <set>

namespace ECS {
    class System {
    public:
        std::set<Entity> entities;
    };
}

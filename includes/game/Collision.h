#pragma once
#include <glm/glm.hpp>

struct AABB {
    glm::vec2 min;
    glm::vec2 max;

    // Controlla la sovrapposizione tra due AABB
    bool isColliding(const AABB& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x &&
            min.y <= other.max.y && max.y >= other.min.y);
    }
};
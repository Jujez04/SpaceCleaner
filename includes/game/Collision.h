#pragma once
#include <glm/glm.hpp>
#include "graphics/Mesh.h"

struct AABB {
    glm::vec2 min;
    glm::vec2 max;

    AABB()
        : min(std::numeric_limits<float>::max()),
        max(std::numeric_limits<float>::lowest()) {
    }   

    AABB(const glm::vec2& minPt, const glm::vec2& maxPt)
        : min(minPt), max(maxPt) {
    }

    // Controlla la sovrapposizione tra due AABB
    bool isColliding(const AABB& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x &&
            min.y <= other.max.y && max.y >= other.min.y);
    }
};

AABB calculateMeshAABB(const std::shared_ptr<Mesh>& mesh, const glm::mat4& transformMatrix);
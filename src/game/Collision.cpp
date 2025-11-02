#include "game/Collision.h"

AABB calculateMeshAABB(const std::shared_ptr<Mesh>& mesh, const glm::mat4& transformMatrix) {
    AABB box;
    const auto& verts = mesh->getVertices();

    for (size_t i = 0; i < verts.size(); i += 3) {
        glm::vec4 vertexPos(verts[i], verts[i + 1], verts[i + 2], 1.0f);
        vertexPos = transformMatrix * vertexPos;

        glm::vec2 xy(vertexPos.x, vertexPos.y);
        box.min = glm::min(box.min, xy);
        box.max = glm::max(box.max, xy);
    }

    return box;
}

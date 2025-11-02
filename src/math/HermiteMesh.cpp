#include "math/HermiteMesh.h"

#include <array>

#include "dependencies/earcut.hpp"
#include "graphics/MeshManager.h"
#include "graphics/Mesh.h"
#include "math/Hermite.h"

unsigned int HermiteMesh::baseHermiteToMesh(const std::string& name, const std::vector<glm::vec2>& controlPoints, const int segmentsPerCurve)
{
    if (controlPoints.size() < 2) return 0;

    float tension = 0.5f;
    std::vector<glm::vec2> tangents = HermiteUtility::calculateTangents(controlPoints, tension);
    std::vector<glm::vec2> curvePoints = HermiteUtility::generateClosedHermiteCurve(controlPoints, tangents, segmentsPerCurve);

    std::vector<std::vector<std::array<double, 2>>> polygon;
    std::vector<std::array<double, 2>> ring;
    for (const auto& pt : curvePoints)
        ring.push_back({ static_cast<double>(pt.x), static_cast<double>(pt.y) });
    polygon.push_back(ring);

    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygon);

    std::vector<float> vertices;
    for (const auto& pt : curvePoints) {
        vertices.push_back(pt.x);
        vertices.push_back(pt.y);
        vertices.push_back(0.0f);
    }

    static unsigned int meshCounter = 0;
    std::string meshName = name + "_mesh_" + std::to_string(meshCounter++);
    glm::vec2 minPoint = curvePoints[0];
    for (size_t i = 1; i < curvePoints.size(); ++i) {
        minPoint = glm::min(minPoint, curvePoints[i]);
    }
    glm::vec2 maxPoint = curvePoints[0];
    for (size_t i = 1; i < curvePoints.size(); ++i) {
        minPoint = glm::max(minPoint, curvePoints[i]);
    }
    std::shared_ptr<Mesh> newMesh = MeshManager::registerMesh(meshName, vertices, indices);
    newMesh->setMin(minPoint);
    newMesh->setMin(maxPoint);
    return newMesh ? newMesh->getId() : 0;
}

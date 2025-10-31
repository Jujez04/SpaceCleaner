#include "game/SpaceCleaner.h"

#include <array>

#include "math/Hermite.h"
#include "graphics/MeshManager.h"
#include "graphics/Mesh.h"
#include "dependencies/earcut.hpp"

SpaceCleaner::SpaceCleaner(const std::string& name)
	: Entity(name), speed(0.0f), direction(0.0f, 0.0f)
{}

void SpaceCleaner::update(float deltaTime)
{
	transform.setPosition(transform.getPosition() + direction * speed * deltaTime);
}

void SpaceCleaner::generateHermiteMesh(const std::vector<glm::vec2>& controlPoints, int segmentsPerCurve)
{
    if (controlPoints.size() < 2) return;

    float tension = 0.5f;
    std::vector<glm::vec2> tangents = HermiteUtility::calculateTangents(controlPoints, tension);
    std::vector<glm::vec2> curvePoints = HermiteUtility::generateClosedHermiteCurve(controlPoints, tangents, segmentsPerCurve);

    std::vector<std::vector<std::array<double, 2>>> polygon;
    std::vector<std::array<double, 2>> ring;
    for (const auto& pt : curvePoints) {
        ring.emplace_back(std::array<double, 2>{ static_cast<double>(pt.x), static_cast<double>(pt.y) });
    }
    polygon.push_back(ring);

    // Genera gli indici con earcut
    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygon);


    // Conversione in array di float per la mesh
    std::vector<float> vertices;
    for (const auto& pt : curvePoints) {
        vertices.push_back(pt.x);
        vertices.push_back(pt.y);
        vertices.push_back(0.0f); // z = 0 per 2D
    }

    std::string meshName = getName() + "_mesh";
    MeshManager::registerMesh(meshName, vertices, indices);

    mesh.setMeshName(meshName);
}

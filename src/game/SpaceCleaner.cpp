#include "game/SpaceCleaner.h"

#include "math/Hermite.h"
#include "graphics/MeshManager.h"
#include "graphics/Mesh.h"

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

    // Conversione in array di float per la mesh
    std::vector<float> vertices;
    for (const auto& pt : curvePoints) {
        vertices.push_back(pt.x);
        vertices.push_back(pt.y);
        vertices.push_back(0.0f); // z = 0 per 2D
        // Aggiungi anche il colore (deve corrispondere al layout dello shader!)
        vertices.push_back(1.0f); // r
        vertices.push_back(1.0f); // g
        vertices.push_back(1.0f); // b
    }

    std::vector<unsigned int> indices;
    for (size_t i = 0; i < curvePoints.size(); ++i) {
        indices.push_back(static_cast<unsigned int>(i));
    }

    std::string meshName = getName() + "_mesh";
    MeshManager::registerMesh(meshName, vertices, indices);

    mesh.setMeshName(meshName);
}

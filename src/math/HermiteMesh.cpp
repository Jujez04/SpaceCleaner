#include "math/HermiteMesh.h"

#include <array>

#include "dependencies/earcut.hpp"
#include "graphics/MeshManager.h"
#include "graphics/Mesh.h"
#include "math/Hermite.h"
#include "utilities/Utilities.h"

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
        minPoint = glm::max(maxPoint, curvePoints[i]);
    }
    std::shared_ptr<Mesh> newMesh = MeshManager::registerMesh(meshName, vertices, indices);
    newMesh->setMin(minPoint);
    newMesh->setMax(maxPoint);
    return newMesh ? newMesh->getId() : 0;
}

unsigned int HermiteMesh::catmullRomToMesh(const std::string& name, const std::string& filePath, int segmentsPerCurve)
{
    std::vector<glm::vec2> controlPoints;
    std::vector<TBCParams> tbcParams;

    // 1. Carica i punti e i parametri T/B/C dal file
    if (!loadCatmullRomFile(filePath, controlPoints, tbcParams)) {
        std::cerr << "[CatmullRomToMesh] Errore: impossibile leggere " << filePath << std::endl;
        return 0;
    }

    if (controlPoints.size() < 2) {
        std::cerr << "[CatmullRomToMesh] Errore: punti insufficienti nel file " << filePath << std::endl;
        return 0;
    }

    // 2. Genera la curva chiusa Catmull-Rom TBC
    std::vector<glm::vec2> curvePoints = CatmullRomUtility::generateCatmullRomTBC(controlPoints, tbcParams, segmentsPerCurve);

    // 3. Prepara i dati per la triangolazione con Earcut
    std::vector<std::vector<std::array<double, 2>>> polygon;
    std::vector<std::array<double, 2>> ring;
    for (const auto& pt : curvePoints)
        ring.push_back({ static_cast<double>(pt.x), static_cast<double>(pt.y) });
    polygon.push_back(ring);

    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygon);

    // 4. Vertici (Z = 0)
    std::vector<float> vertices;
    for (const auto& pt : curvePoints) {
        vertices.push_back(pt.x);
        vertices.push_back(pt.y);
        vertices.push_back(0.0f);
    }

    // 5. Calcolo AABB
    glm::vec2 minPt = curvePoints[0], maxPt = curvePoints[0];
    for (const auto& p : curvePoints) {
        minPt = glm::min(minPt, p);
        maxPt = glm::max(maxPt, p);
    }

    // 6. Registra la mesh nel MeshManager
    static unsigned int meshCounter = 0;
    std::string meshName = name + "_catmullrom_" + std::to_string(meshCounter++);
    std::shared_ptr<Mesh> newMesh = MeshManager::registerMesh(meshName, vertices, indices);

    if (newMesh) {
        newMesh->setMin(minPt);
        newMesh->setMax(maxPt);
        return newMesh->getId();
    }

    return 0;
}

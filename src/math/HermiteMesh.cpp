#include "math/HermiteMesh.h"
#include <array>
#include "dependencies/earcut.hpp"
#include "graphics/MeshManager.h"
#include "graphics/Mesh.h"
#include "math/Hermite.h"
#include "utilities/Utilities.h"

/**
 * @brief Genera una mesh triangolata a partire da una curva di Hermite chiusa.
 *
 * 1. Calcola le tangenti dei punti di controllo usando HermiteUtility::calculateTangents.
 * 2. Genera i punti della curva chiusa con HermiteUtility::generateClosedHermiteCurve.
 * 3. Prepara la struttura dati per Earcut (vettore di anelli, con coordinate x,y).
 * 4. Chiama Earcut per ottenere gli indici dei triangoli.
 * 5. Converte i punti 2D in vertici 3D con z=0.
 * 6. Calcola AABB della mesh.
 * 7. Registra la mesh nel MeshManager e ritorna l'ID.
 *
 * @param name Nome base della mesh.
 * @param controlPoints Punti di controllo della curva.
 * @param segmentsPerCurve Numero di segmenti per tratto Hermite.
 * @return ID della mesh registrata, oppure 0 se fallisce.
 */
unsigned int HermiteMesh::baseHermiteToMesh(const std::string& name, const std::vector<glm::vec2>& controlPoints, const int segmentsPerCurve)
{
    if (controlPoints.size() < 2) return 0;

    float tension = 0.5f; // Tensione Hermite standard
    // Calcola tangenti dei punti per generare una curva Hermite liscia
    std::vector<glm::vec2> tangents = HermiteUtility::calculateTangents(controlPoints, tension);

    // Genera punti della curva chiusa (interpolazione Hermite)
    std::vector<glm::vec2> curvePoints = HermiteUtility::generateClosedHermiteCurve(controlPoints, tangents, segmentsPerCurve);

    // Prepara dati per triangolazione Earcut
    std::vector<std::vector<std::array<double, 2>>> polygon;
    std::vector<std::array<double, 2>> ring;
    for (const auto& pt : curvePoints)
        ring.push_back({ static_cast<double>(pt.x), static_cast<double>(pt.y) });
    polygon.push_back(ring);

    // Genera indici dei triangoli usando Earcut
    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygon);

    // Converte i punti 2D in vertici 3D con z=0
    std::vector<float> vertices;
    for (const auto& pt : curvePoints) {
        vertices.push_back(pt.x);
        vertices.push_back(pt.y);
        vertices.push_back(0.0f);
    }

    // Calcolo AABB (min/max) per bounding box della mesh
    glm::vec2 minPoint = curvePoints[0];
    glm::vec2 maxPoint = curvePoints[0];
    for (size_t i = 1; i < curvePoints.size(); ++i) {
        minPoint = glm::min(minPoint, curvePoints[i]);
        maxPoint = glm::max(maxPoint, curvePoints[i]);
    }

    // Registra la mesh nel MeshManager
    static unsigned int meshCounter = 0;
    std::string meshName = name + "_mesh_" + std::to_string(meshCounter++);
    std::shared_ptr<Mesh> newMesh = MeshManager::registerMesh(meshName, vertices, indices);

    if (newMesh) {
        newMesh->setMin(minPoint);
        newMesh->setMax(maxPoint);
        return newMesh->getId();
    }

    return 0;
}

/**
 * @brief Genera una mesh triangolata da una curva Kochanek-Bartels / TBC (ex Catmull-Rom generalizzata).
 *
 * 1. Carica punti e parametri Tension/Bias/Continuity da file con loadCatmullRomFile.
 * 2. Genera i punti della curva chiusa usando SplineUtility::generateTBC.
 * 3. Prepara dati per triangolazione Earcut.
 * 4. Calcola vertici 3D (z=0).
 * 5. Calcola AABB della mesh.
 * 6. Registra la mesh nel MeshManager e ritorna l'ID.
 *
 * @param name Nome base della mesh.
 * @param filePath Percorso del file contenente punti e parametri TBC.
 * @param segmentsPerCurve Numero di segmenti per ogni tratto della curva.
 * @return ID della mesh registrata, oppure 0 in caso di errore.
 */
unsigned int HermiteMesh::catmullRomToMesh(const std::string& name, const std::string& filePath, int segmentsPerCurve)
{
    std::vector<glm::vec2> controlPoints;
    std::vector<TBCParams> tbcParams;

    // Carica punti e parametri T/B/C dal file
    if (!loadCatmullRomFile(filePath, controlPoints, tbcParams)) {
        std::cerr << "[CatmullRomToMesh] Errore: impossibile leggere " << filePath << std::endl;
        return 0;
    }

    if (controlPoints.size() < 2) {
        std::cerr << "[CatmullRomToMesh] Errore: punti insufficienti nel file " << filePath << std::endl;
        return 0;
    }

    // Genera la curva chiusa Kochanek-Bartels
    std::vector<glm::vec2> curvePoints = SplineUtility::generateTBC(controlPoints, tbcParams, segmentsPerCurve);

    // Prepara i dati per la triangolazione con Earcut
    std::vector<std::vector<std::array<double, 2>>> polygon;
    std::vector<std::array<double, 2>> ring;
    for (const auto& pt : curvePoints)
        ring.push_back({ static_cast<double>(pt.x), static_cast<double>(pt.y) });
    polygon.push_back(ring);

    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygon);

    // Genera vertici 3D (z=0)
    std::vector<float> vertices;
    for (const auto& pt : curvePoints) {
        vertices.push_back(pt.x);
        vertices.push_back(pt.y);
        vertices.push_back(0.0f);
    }

    // Calcolo AABB della mesh
    glm::vec2 minPt = curvePoints[0], maxPt = curvePoints[0];
    for (const auto& p : curvePoints) {
        minPt = glm::min(minPt, p);
        maxPt = glm::max(maxPt, p);
    }

    // Registra la mesh
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

#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

/**
 * @class HermiteMesh
 * @brief Genera mesh triangolate a partire da curve di Hermite o Kochanek–Bartels.
 *
 * Questa classe fornisce funzioni per convertire una curva parametrica (Hermite o TBC)
 * in una mesh triangolata 2D, utilizzando l’algoritmo Earcut per la triangolazione.
 */
class HermiteMesh {
public:
    /**
     * @brief Genera una mesh triangolata a partire da una curva di Hermite chiusa.
     * @param name Nome base della mesh.
     * @param controlPoints Punti di controllo della curva.
     * @param segmentsPerCurve Numero di segmenti per tratto.
     * @return ID univoco della mesh registrata nel MeshManager, oppure 0 se fallisce.
     */
    static unsigned int baseHermiteToMesh(
        const std::string& name,
        const std::vector<glm::vec2>& controlPoints,
        const int segmentsPerCurve = 40);

    /**
     * @brief Genera una mesh triangolata da una curva TBC (ex Catmull-Rom generalizzata).
     * @param name Nome base della mesh.
     * @param filePath Percorso del file contenente punti e parametri TBC.
     * @param segmentsPerCurve Numero di segmenti per tratto.
     * @return ID univoco della mesh registrata, oppure 0 se si verifica un errore.
     */
    static unsigned int catmullRomToMesh(
        const std::string& name,
        const std::string& filePath,
        int segmentsPerCurve = 50);
};

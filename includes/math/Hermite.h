#pragma once
#include <glm/glm.hpp>
#include <vector>

class HermiteUtility {
public:
    // Genera punti lungo una curva di Hermite
    static std::vector<glm::vec2> generateHermiteCurve(
        glm::vec2 p0, glm::vec2 p1,      // Punti di controllo
        glm::vec2 t0, glm::vec2 t1,      // Tangenti
        int segments = 20);

    // Genera una curva chiusa (per forme)
    static std::vector<glm::vec2> generateClosedHermiteCurve(
        const std::vector<glm::vec2>& controlPoints,
        const std::vector<glm::vec2>& tangents,
        int segmentsPerCurve = 20);

    // Calcola tangenti automaticamente (Catmull-Rom style)
    static std::vector<glm::vec2> calculateTangents(
        const std::vector<glm::vec2>& controlPoints,
        float tension = 0.5f);

private:
    // Funzioni base di Hermite
    static float hP0(float t);
    static float hP1(float t);
    static float hT0(float t);
    static float hT1(float t);

    // Funzione per calcolare la curva di Hermite tra due punti in un intervallo [0, 1]
    static glm::vec2 hermite(float t, const glm::vec2& p0, const glm::vec2& p1,
        const glm::vec2& t0, const glm::vec2& t1);
};
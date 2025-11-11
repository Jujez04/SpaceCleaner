#include "math/Hermite.h"
#include <cmath>

/**
 * @brief Funzione base di Hermite
 */
float HermiteUtility::phi0(float t) {
    return 2 * t * t * t - 3 * t * t + 1;
}

/**
 * @brief Funzione base di Hermite
 */
float HermiteUtility::phi1(float t) {
    return -2 * t * t * t + 3 * t * t;
}

/**
 * @brief Funzione base di Hermite
 */
float HermiteUtility::psi0(float t) {
    return t * t * t - 2 * t * t + t;
}

/**
 * @brief Funzione base di Hermite
 */
float HermiteUtility::psi1(float t) {
    return t * t * t - t * t;
}

/**
 * @brief Valuta la curva di Hermite interpolata in un parametro t in [0, 1].
 */
glm::vec2 HermiteUtility::hermite(float t, const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& t0, const glm::vec2& t1) {
    return phi0(t) * p0 + phi1(t) * p1 + psi0(t) * t0 + psi1(t) * t1;
}

/**
 * @brief Genera una serie di punti lungo la curva di Hermite.
 *
 * Divide l'intervallo [0,1] in `segments` parti e valuta la curva in ciascun punto.
 */
std::vector<glm::vec2> HermiteUtility::generateHermiteCurve(glm::vec2 p0, glm::vec2 p1, glm::vec2 t0, glm::vec2 t1, int segments) {
    std::vector<glm::vec2> curvePoints;
    for (int i = 0; i <= segments; ++i) {
        float t = static_cast<float>(i) / segments;
        curvePoints.push_back(hermite(t, p0, p1, t0, t1));
    }
    return curvePoints;
}

/**
 * @brief Genera una curva chiusa concatenando segmenti di Hermite tra punti successivi.
 *
 * Le tangenti vengono riutilizzate ciclicamente, garantendo la chiusura geometrica.
 * Ogni segmento è campionato in `segmentsPerCurve` punti.
 */
std::vector<glm::vec2> HermiteUtility::generateClosedHermiteCurve(const std::vector<glm::vec2>& controlPoints, const std::vector<glm::vec2>& tangents, int segmentsPerCurve) {
    int n = controlPoints.size();
    std::vector<glm::vec2> closedCurve;

    for (int i = 0; i < n; i++) {
        glm::vec2 p0 = controlPoints[i];
        glm::vec2 p1 = controlPoints[(i + 1) % n];
        glm::vec2 t0 = tangents[i];
        glm::vec2 t1 = tangents[(i + 1) % n];

        std::vector<glm::vec2> segment = generateHermiteCurve(p0, p1, t0, t1, segmentsPerCurve);
        closedCurve.insert(closedCurve.end(), segment.begin(), segment.end() - 1);
    }
    return closedCurve;
}

/**
 * @brief Calcola tangenti automatiche per una sequenza di punti di controllo.
 */
std::vector<glm::vec2> HermiteUtility::calculateTangents(const std::vector<glm::vec2>& controlPoints, float tension) {
    std::vector<glm::vec2> tangents;
    int n = controlPoints.size();

    for (int i = 0; i < n; i++) {
        glm::vec2 pPrev = controlPoints[(i - 1 + n) % n];
        glm::vec2 pNext = controlPoints[(i + 1) % n];
        glm::vec2 tangent = (1.0f - tension) * 0.5f * (pNext - pPrev);
        tangents.push_back(tangent);
    }

    return tangents;
}

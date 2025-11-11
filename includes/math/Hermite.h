#pragma once
#include <glm/glm.hpp>
#include <vector>

/**
 * @class HermiteUtility
 * @brief Classe statica che fornisce funzioni per la generazione di curve di Hermite.
 *
 * Le curve di Hermite sono definite da due punti di controllo e le rispettive tangenti.
 * Questa classe fornisce anche metodi per generare curve chiuse e calcolare tangenti automatiche.
 */
class HermiteUtility {
public:
    /**
     * @brief Genera una curva di Hermite tra due punti.
     * @param p0 Punto iniziale.
     * @param p1 Punto finale.
     * @param t0 Tangente in uscita da p0.
     * @param t1 Tangente in ingresso a p1.
     * @param segments Numero di punti campionati lungo la curva.
     * @return Vettore di punti 2D che descrivono la curva.
     */
    static std::vector<glm::vec2> generateHermiteCurve(
        glm::vec2 p0, glm::vec2 p1,
        glm::vec2 t0, glm::vec2 t1,
        int segments = 20);

    /**
     * @brief Genera una curva di Hermite chiusa con continuit� C1.
     * @param controlPoints Punti di controllo.
     * @param tangents Tangenti associate ad ogni punto.
     * @param segmentsPerCurve Numero di segmenti per ogni tratto.
     * @return Punti della curva chiusa risultante.
     */
    static std::vector<glm::vec2> generateClosedHermiteCurve(
        const std::vector<glm::vec2>& controlPoints,
        const std::vector<glm::vec2>& tangents,
        int segmentsPerCurve = 20);

    /**
     * @brief Calcola automaticamente le tangenti secondo la formulazione Catmull�Rom.
     * @param controlPoints Punti di controllo.
     * @param tension Parametro di tensione (0.5f = Catmull-Rom standard).
     * @return Vettore di tangenti calcolate.
     */
    static std::vector<glm::vec2> calculateTangents(
        const std::vector<glm::vec2>& controlPoints,
        float tension = 0.5f);

private:
    static float phi0(float t); ///< Base Hermite per p0.
    static float phi1(float t); ///< Base Hermite per p1.
    static float psi0(float t); ///< Base Hermite per t0.
    static float psi1(float t); ///< Base Hermite per t1.

    /**
     * @brief Calcola un punto sulla curva di Hermite dato t ? [0, 1].
     * @param t Parametro normalizzato (0 = inizio, 1 = fine).
     * @param p0 Punto iniziale.
     * @param p1 Punto finale.
     * @param t0 Tangente iniziale.
     * @param t1 Tangente finale.
     * @return Punto 2D calcolato.
     */
    static glm::vec2 hermite(float t, const glm::vec2& p0, const glm::vec2& p1,
        const glm::vec2& t0, const glm::vec2& t1);
};

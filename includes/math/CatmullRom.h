#pragma once
#include <glm/glm.hpp>
#include <vector>

/**
 * @struct TBCParams
 * @brief Parametri della curva Kochanek–Bartels (Tension, Bias, Continuity).
 */
struct TBCParams {
    float tension;     ///< Controlla la "morbidezza" della curva (0 = standard Catmull-Rom).
    float bias;        ///< Controlla la direzione della tangente (positivo verso il punto precedente).
    float continuity;  ///< Controlla la continuità tra segmenti (0 = continua, ±1 = spigoli netti).
};

/**
 * @class SplineUtility
 * @brief Classe statica che fornisce funzioni per generare curve spline Kochanek–Bartels (TBC).
 *
 * Questa classe implementa funzioni di supporto per il calcolo di tangenti e la generazione
 * di curve Tension–Bias–Continuity, che generalizzano le Catmull-Rom.
 */
class SplineUtility {
public:
    /**
     * @brief Calcola la tangente in un punto secondo la formulazione di Kochanek–Bartels.
     *
     * @param prev Punto precedente.
     * @param curr Punto corrente.
     * @param next Punto successivo.
     * @param tension Controlla la tensione della curva (0 = morbida, 1 = rigida).
     * @param bias Controlla l'orientamento della tangente.
     * @param continuity Controlla la continuità tra segmenti.
     * @param isOutgoing True se la tangente è in uscita dal punto corrente, False se in ingresso.
     * @return La tangente calcolata come vettore 2D.
     */
    static glm::vec2 kochanekBartelsTangent(
        const glm::vec2& prev,
        const glm::vec2& curr,
        const glm::vec2& next,
        float tension, float bias, float continuity,
        bool isOutgoing
    );

    /**
     * @brief Genera una curva Tension–Bias–Continuity chiusa (TBC spline).
     *
     * @param controlPoints Punti di controllo della curva.
     * @param params Parametri TBC per ogni punto.
     * @param segmentsPerCurve Numero di segmenti da generare per ciascuna porzione di curva.
     * @return Un vettore di punti campionati sulla curva spline risultante.
     */
    static std::vector<glm::vec2> generateTBC(
        const std::vector<glm::vec2>& controlPoints,
        const std::vector<TBCParams>& params,
        int segmentsPerCurve
    );
};

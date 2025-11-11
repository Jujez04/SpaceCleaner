#include "math/CatmullRom.h"
#include "math/Hermite.h"

/**
 * @brief Calcola la tangente entrante o uscente di un punto secondo il modello di Kochanek–Bartels (Tension–Bias–Continuity).
 *
 * Questa funzione definisce come la curva entra o esce da un punto di controllo,
 * regolando la forma locale tramite i parametri:
 *  - **tension**: regola la "rigidità" della curva (valori alti rendono la curva più dritta);
 *  - **bias**: controlla la direzione della tangente (positivo = verso il punto precedente);
 *  - **continuity**: gestisce la continuità C¹ tra segmenti (influenza l’angolo di raccordo).
 *
 * La formula è una combinazione lineare dei vettori (curr - prev) e (next - curr),
 * pesata dai tre parametri per ottenere una transizione regolare o più spigolosa.
 */
glm::vec2 SplineUtility::kochanekBartelsTangent(
    const glm::vec2& prev,
    const glm::vec2& curr,
    const glm::vec2& next,
    float tension, float bias, float continuity,
    bool isOutgoing
) {
    // Fattore di scala comune a entrambe le direzioni
    float mul = (1.0f - tension) * 0.5f;

    // Se la tangente è uscente o entrante, i segni dei termini cambiano
    if (isOutgoing) {
        return mul * ((1 + bias) * (1 + continuity) * (curr - prev) +
            (1 - bias) * (1 - continuity) * (next - curr));
    }
    else {
        return mul * ((1 + bias) * (1 - continuity) * (curr - prev) +
            (1 - bias) * (1 + continuity) * (next - curr));
    }
}

/**
 * @brief Genera una curva chiusa usando il modello Tension–Bias–Continuity (Kochanek–Bartels).
 *
 * Ogni coppia di punti consecutivi viene interpolata da una curva di Hermite,
 * dove le tangenti sono calcolate con la funzione `kochanekBartelsTangent()`.
 *
 * L’idea è combinare più curve di Hermite continue per ottenere una curva chiusa,
 * dove ciascun segmento conserva le proprietà di tensione, bias e continuità definite localmente.
 *
 * @note L’ultimo punto viene collegato al primo per garantire una chiusura geometrica.
 */
std::vector<glm::vec2> SplineUtility::generateTBC(
    const std::vector<glm::vec2>& controlPoints,
    const std::vector<TBCParams>& params,
    int segmentsPerCurve
) {
    std::vector<glm::vec2> curvePoints;
    int n = static_cast<int>(controlPoints.size());

    for (int i = 0; i < n; ++i) {
        // Indici modulari per mantenere la curva chiusa
        glm::vec2 p0 = controlPoints[(i - 1 + n) % n];
        glm::vec2 p1 = controlPoints[i];
        glm::vec2 p2 = controlPoints[(i + 1) % n];
        glm::vec2 p3 = controlPoints[(i + 2) % n];

        // Parametri TBC locali ai due punti
        const auto& par1 = params[i];
        const auto& par2 = params[(i + 1) % n];

        // Calcolo delle tangenti di Hermite in base a Kochanek–Bartels
        glm::vec2 t1 = kochanekBartelsTangent(p0, p1, p2, par1.tension, par1.bias, par1.continuity, true);
        glm::vec2 t2 = kochanekBartelsTangent(p1, p2, p3, par2.tension, par2.bias, par2.continuity, false);

        // Segmento Hermite interpolato tra p1 e p2
        auto segment = HermiteUtility::generateHermiteCurve(p1, p2, t1, t2, segmentsPerCurve);

        // Inserisce i punti tranne l'ultimo, per evitare duplicati ai giunti
        curvePoints.insert(curvePoints.end(), segment.begin(), segment.end() - 1);
    }

    return curvePoints;
}

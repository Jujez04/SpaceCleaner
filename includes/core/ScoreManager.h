#pragma once

/**
 * @class ScoreManager
 * @brief Gestisce il punteggio del giocatore durante il gioco
 *
 * @note Il punteggio non può mai diventare negativo (clamped a 0)
 */
class ScoreManager {
public:

    /**
     * @brief Costruttore di default
     *
     * Inizializza il punteggio a 0 all'inizio del gioco.
     * Utilizza la initialization list per efficienza.
     */
    ScoreManager() : score(0) {}

    /**
     * @brief Ritorna il punteggio corrente
     * @return Punteggio attuale del giocatore
     */
    int getScore() const {
        return score;
    }

    /**
     * @brief Modifica il punteggio aggiungendo o sottraendo punti
     * @param delta Quantità di punti da aggiungere (positivo) o sottrarre (negativo)
     *
     * Comportamento:
     * - Se delta > 0: incrementa il punteggio (es. +100 per uccisione nemico)
     * - Se delta < 0: decrementa il punteggio (es. -50 per danno subito)
     * - Se il risultato sarebbe negativo, il punteggio viene tagliato a 0
     */
    void adjustScore(int delta) {
        score += delta;

        if (score < 0) {
            score = 0;
        }
    }

    /**
     * @brief Azzera completamente il punteggio
     *
     * Utilizzato tipicamente:
     * - All'inizio di una nuova partita
     * - Quando il giocatore preme "Restart" dopo Game Over
     * - Per resettare lo stato dopo un test/debug
     *
     * @see Engine::resetGame() - Chiama questo metodo al reset
     */
    void reset() {
        score = 0;
    }

private:

    /**
     * @brief Punteggio corrente del giocatore
     *
     * Invarianti:
     * - Sempre >= 0 (garantito da adjustScore)
     * - Inizializzato a 0 nel costruttore
     * - Accessibile solo tramite metodi pubblici (incapsulamento)
     */
    int score;
};
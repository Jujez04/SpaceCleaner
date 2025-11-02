#pragma once

class ScoreManager {
public:
    ScoreManager() : score(0) {}

    // Ritorna il punteggio corrente
    int getScore() const { return score; }

    // Modifica il punteggio
    void adjustScore(int delta) {
        score += delta;
        if (score < 0) {
            score = 0;
        }
    }

    // Azzera il punteggio
    void reset() {
        score = 0;
    }

private:
    int score;
};
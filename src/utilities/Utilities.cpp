#include "utilities/Utilities.h"

/**
 * @brief Legge l'intero contenuto di un file di testo.
 *
 * Apre il file, legge tutto il contenuto in un stringstream e lo ritorna
 * come std::string. Se il file non può essere aperto, scrive un messaggio
 * di errore su std::cerr e ritorna stringa vuota.
 */
std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Errore nell'aprire il file: " << filePath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

/**
 * @brief Carica punti e parametri TBC da un file Catmull-Rom.
 *
 * Legge riga per riga. Ogni riga deve avere 6 valori:
 * x y z tension bias continuity
 * - x,y: coordinate 2D del punto
 * - z: ignorato
 * - tension, bias, continuity: parametri per Kochanek-Bartels TBC
 *
 * I punti vengono memorizzati in `points` e i parametri corrispondenti in `params`.
 * Righe malformate vengono ignorate.
 *
 * @param path Percorso del file da leggere.
 * @param points Vettore di output dei punti 2D.
 * @param params Vettore di output dei parametri TBC.
 * @return true se il file è stato caricato correttamente, false altrimenti.
 */
bool loadCatmullRomFile(const std::string& path,
    std::vector<glm::vec2>& points,
    std::vector<TBCParams>& params)
{
    std::ifstream file(path);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        float x, y, z, t, b, c;
        if (!(iss >> x >> y >> z >> t >> b >> c)) continue; // ignora righe non valide
        points.emplace_back(x, y);
        params.push_back({ t, b, c });
    }
    return true;
}

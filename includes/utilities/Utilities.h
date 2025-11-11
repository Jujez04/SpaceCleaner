#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "math/CatmullRom.h"

/**
 * @brief Legge l'intero contenuto di un file di testo.
 * @param filePath Percorso del file da leggere.
 * @return Il contenuto del file come stringa. Ritorna stringa vuota in caso di errore.
 */
std::string readFile(const std::string& filePath);

/**
 * @brief Carica punti e parametri Tension/Bias/Continuity (TBC) da un file Catmull-Rom.
 *
 * Ogni riga del file deve contenere:
 * x y z tension bias continuity
 * dove x,y,z sono le coordinate del punto (z viene ignorato)
 * e tension, bias, continuity sono valori per la curva TBC.
 *
 * @param path Percorso del file da leggere.
 * @param points Vettore di output dei punti 2D (x,y) della curva.
 * @param params Vettore di output dei parametri TBC corrispondenti ai punti.
 * @return true se il file è stato caricato correttamente, false altrimenti.
 */
bool loadCatmullRomFile(const std::string& path, std::vector<glm::vec2>& points, std::vector<TBCParams>& params);

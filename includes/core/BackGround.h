#pragma once
#include <string>

/**  
 * @brief Struttura per configurare il background
 * Associa un nome allo shader -> trova uso in ShaderManager
 * @param name Nome della configurazione
 * @param fragmentShaderPath 
 * @param shaderId
*/
struct BackgroundConfig {
    std::string name;             
    std::string fragmentShaderPath;
    unsigned int shaderId = 0;
};
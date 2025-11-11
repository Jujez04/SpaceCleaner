/**
 * @file ShaderManager.h
 * @brief Gestore centralizzato per la creazione e l’accesso agli shader.
 */

#pragma once
#include <memory>
#include <string>
#include <unordered_map>

class Shader;

/**
 * @class ShaderManager
 * @brief Fornisce un meccanismo statico per caricare, gestire e recuperare shader.
 */
class ShaderManager {
public:
    ShaderManager() = default;
    ~ShaderManager() = default;

    /**
     * @brief Carica uno shader dai file sorgente e lo registra nel sistema.
     * @param name Nome logico dello shader.
     * @param vertexPath Percorso del file shader vertex.
     * @param fragmentPath Percorso del file shader fragment.
     * @return ID univoco assegnato allo shader.
     */
    static unsigned int load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);

    /** @brief Restituisce uno shader tramite nome. */
    static std::shared_ptr<Shader> get(const std::string& name);

    /** @brief Restituisce uno shader tramite ID. */
    static std::shared_ptr<Shader> get(unsigned int id);

    /** @brief Restituisce l’ID associato a un nome di shader. */
    static unsigned int getId(const std::string& name);

private:
    static std::unordered_map<std::string, std::shared_ptr<Shader>> shadersMap; ///< Mappa nome ? Shader.
    static std::unordered_map<unsigned int, std::shared_ptr<Shader>> idShadersMap; ///< Mappa ID ? Shader.
    static std::unordered_map<std::string, unsigned int> nameToIdMap; ///< Mappa nome ? ID.
    static unsigned int nextId; ///< Contatore per assegnare nuovi ID.
};

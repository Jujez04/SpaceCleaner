/**
 * @file ShaderManager.cpp
 * @brief Implementazione del gestore centralizzato degli shader.
 */

#include "graphics/ShaderManager.h"
#include "graphics/Shader.h"
#include <stdexcept>

std::unordered_map<std::string, std::shared_ptr<Shader>> ShaderManager::shadersMap;
std::unordered_map<unsigned int, std::shared_ptr<Shader>> ShaderManager::idShadersMap;
std::unordered_map<std::string, unsigned int> ShaderManager::nameToIdMap;
unsigned int ShaderManager::nextId = 1;

/**
 * @brief Carica e registra un nuovo shader se non esiste già.
 */
unsigned int ShaderManager::load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
{
    auto it = nameToIdMap.find(name);
    if (it != nameToIdMap.end()) return it->second;

    auto shader = std::make_shared<Shader>(vertexPath, fragmentPath);
    unsigned int newId = nextId++;
    shadersMap[name] = shader;
    idShadersMap[newId] = shader;
    nameToIdMap[name] = newId;

    return newId;
}

/**
 * @brief Recupera uno shader tramite nome.
 */
std::shared_ptr<Shader> ShaderManager::get(const std::string& name)
{
    auto it = shadersMap.find(name);
    return it != shadersMap.end() ? it->second : nullptr;
}

/**
 * @brief Recupera uno shader tramite ID.
 */
std::shared_ptr<Shader> ShaderManager::get(unsigned int id)
{
    auto it = idShadersMap.find(id);
    return it != idShadersMap.end() ? it->second : nullptr;
}

/**
 * @brief Restituisce l’ID associato al nome dello shader.
 */
unsigned int ShaderManager::getId(const std::string& name)
{
    auto it = nameToIdMap.find(name);
    return it != nameToIdMap.end() ? it->second : 0;
}

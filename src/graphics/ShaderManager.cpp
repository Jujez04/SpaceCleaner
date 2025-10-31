#include "graphics/ShaderManager.h"
#include "graphics/Shader.h"
#include <stdexcept>

std::shared_ptr<Shader> ShaderManager::load(const std::string& name,
    const std::string& vertexPath,
    const std::string& fragmentPath)
{
    // Se esiste già, ritorna la shader esistente
    auto it = shadersMap.find(name);
    if (it != shadersMap.end())
        return it->second;

    // Altrimenti creo e registro la nuova shader
    auto shader = std::make_shared<Shader>(vertexPath, fragmentPath);
    shadersMap[name] = shader;
    return shader;
}

std::shared_ptr<Shader> ShaderManager::get(const std::string& name)
{
    auto it = shadersMap.find(name);
    if (it != shadersMap.end())
        return it->second;

    // Se non esiste, ritorno nullptr (puoi anche lanciare eccezione)
    return nullptr;
}

#include "graphics/ShaderManager.h"
#include "graphics/Shader.h"
#include <stdexcept>

std::unordered_map<std::string, std::shared_ptr<Shader>> ShaderManager::shadersMap;
std::unordered_map<unsigned int, std::shared_ptr<Shader>> ShaderManager::idShadersMap;
std::unordered_map<std::string, unsigned int> ShaderManager::nameToIdMap;
unsigned int ShaderManager::nextId = 1;

unsigned int ShaderManager::load(const std::string& name,
    const std::string& vertexPath,
    const std::string& fragmentPath)
{
    auto it = nameToIdMap.find(name);
    if (it != nameToIdMap.end()) {
        return it->second;
    }

    auto shader = std::make_shared<Shader>(vertexPath, fragmentPath);
    unsigned int newId = nextId++;
    shadersMap[name] = shader;
    idShadersMap[newId] = shader;
    nameToIdMap[name] = newId;

    return newId;
}


std::shared_ptr<Shader> ShaderManager::get(const std::string& name)
{
    auto it = shadersMap.find(name);
    return it != shadersMap.end() ? it->second : nullptr;
}

std::shared_ptr<Shader> ShaderManager::get(unsigned int id)
{
    auto it = idShadersMap.find(id);
    return it != idShadersMap.end() ? it->second : nullptr;
}

unsigned int ShaderManager::getId(const std::string& name)
{
    auto it = nameToIdMap.find(name);
    return it != nameToIdMap.end() ? it->second : 0;
}
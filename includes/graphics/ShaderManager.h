// graphics/ShaderManager.h (Corretto)

#pragma once

#include <memory>
#include <string>
#include <unordered_map>

class Shader;

class ShaderManager {
public:
    ShaderManager() = default;
    ~ShaderManager() = default;

    static unsigned int load(const std::string& name,
        const std::string& vertexPath,
        const std::string& fragmentPath);

    static std::shared_ptr<Shader> get(const std::string& name);

    static std::shared_ptr<Shader> get(unsigned int id);

    static unsigned int getId(const std::string& name);

private:
    static std::unordered_map<std::string, std::shared_ptr<Shader>> shadersMap;
    static std::unordered_map<unsigned int, std::shared_ptr<Shader>> idShadersMap;
    static std::unordered_map<std::string, unsigned int> nameToIdMap;
    static unsigned int nextId;
};
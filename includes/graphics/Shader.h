#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <unordered_map>


class Shader {
private:
	unsigned int rendererId;
	// Cache per i nomi delle uniform
	// Al posto di compilare 2 o più volte lo stesso programma
	// ottengo direttamente l'id del programma che voglio usare
	mutable std::unordered_map<std::string, int> uniformLocationCache;

public:
	Shader() = default;
	Shader(const std::string& vertexShader, const std::string& fragmentShader);
	~Shader();
    void bind() const;
    void unbind();

    // Uniform setters
    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void setUniformVec4(const std::string& name, const glm::vec4& vec);
    void setUniformMat4(const std::string& name, const glm::mat4& matrix);
    void setUniform1f(const std::string& name, float value);
    void setUniformVec2(const std::string& name, const glm::vec2& vec);
    // Utility
    unsigned int getId() const { return rendererId; }
    bool isValid() const { return rendererId != 0; }

private:
    bool compileShader(unsigned int shader, const std::string& type);
    unsigned int createShader(const std::string& vertexCode, const std::string& fragmentCode);
    int getUniformLocationCached(const std::string& name) const;
};

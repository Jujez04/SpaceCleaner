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

	void assignId() {
		rendererId = glCreateProgram();
	}

public:
	Shader() = default;
	Shader(const std::string& vertexShader, const std::string& fragmentShader);
	~Shader();

	void bind();
	void unbind();
	int getUniformLocationCached(const std::string& name) const;
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setUniformVec4(const std::string& name, glm::vec4 vec);
	void setUniformMat4(const std::string& name, const glm::mat4& matrix);
private:
	bool compileShader(unsigned int shader, const std::string& type);
	unsigned int createShader(const std::string& vertexCode, const std::string& fragmentCode);
	
};

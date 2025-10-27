#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/vec4.hpp>
#include <glad/glad.h>

class Shader {
private:
	unsigned int id;

	void assignId() {
		id = glCreateProgram();
	}

public:
	Shader() = default;
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader() = default;

	void setUniformMat4(const std::string& name, float v0, float v1, float v2, float v3);

	void use();

	unsigned int getID() const {
		return id;
	}
};

#endif
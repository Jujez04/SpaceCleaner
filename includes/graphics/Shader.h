#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>


struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;

	ShaderProgramSource(const std::string& vertexSrc, const std::string& fragmentSrc)
		: VertexSource(vertexSrc), FragmentSource(fragmentSrc) {
	}
};

class Shader {
private:
	unsigned int rendererId;

	void assignId() {
		rendererId = glCreateProgram();
	}

public:
	Shader() = default;
	Shader(const std::string& vertexShader, const std::string& fragmentShader);
	~Shader();

	void bind();
	void unbind();

	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setMat4(const std::string& name, const glm::mat4& matrix);
private:
	unsigned int getUniformLocation(const std::string& name);
	bool compileShader(unsigned int shader, const std::string& type);
	unsigned int createShader(const std::string& vertexCode, const std::string& fragmentCode);
	
};

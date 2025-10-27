#pragma once

#include <memory>
#include <string>

class Shader;
class ShaderManager;

class ShaderHandler {
public:
	ShaderHandler();
	~ShaderHandler();

	void useShader(const std::string& name);

	void setUniformMat4(const std::string& name, const float* value);

private:
	ShaderManager& shaderManager;
	std::shared_ptr<Shader> currentShader;
};

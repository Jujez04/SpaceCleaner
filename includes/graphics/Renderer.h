#pragma once
#include <memory>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader;
class Mesh;

class Renderer {
private:
	std::vector<std::shared_ptr<Mesh>> meshes;
	std::unique_ptr<Shader> shader;
	glm::mat4 projection;
	glm::mat4 view;
public:
	Renderer() = default;
	Renderer(const std::string& vertexShader, const std::string& fragmentShader);
	~Renderer();

	void addMesh(const std::shared_ptr<Mesh>& mesh);
	void clear();
	void setCamera(const glm::mat4& viewMat, const glm::mat4& projMat);
	void drawAll(GLenum mode);
};
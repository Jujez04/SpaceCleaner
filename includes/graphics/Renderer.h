#pragma once
#include <memory>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Mesh;
class MeshManager;
class ShaderManager;
class Entity;
class AABB;

class Renderer {
private:
	std::vector<std::shared_ptr<Mesh>> meshes;
	glm::mat4 projection;
	glm::mat4 view;
public:
	Renderer();
	~Renderer() = default;

	void addMesh(const std::shared_ptr<Mesh>& mesh);
	void clear();
	void setCamera(const glm::mat4& viewMat, const glm::mat4& projMat);
	void drawMesh(unsigned int meshId, unsigned int shaderId, const glm::vec4& color, const glm::mat4& model, GLenum mode);
	void drawEntityByInfo(Entity& entity, GLenum mode = GL_TRIANGLES);

	void drawBoundingBox(const AABB& box, const glm::vec4& color, const glm::mat4& view, const glm::mat4& projection);
};
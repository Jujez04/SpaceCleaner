#pragma once
#include <memory>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader;

namespace vrtx {
	class VertexArray;
	class VertexBuffer;
}

class Renderer {
private:
	std::unique_ptr<vrtx::VertexArray> vertexArray;
	std::unique_ptr<vrtx::VertexBuffer> vertexBuffer;
	std::unique_ptr<Shader> shader;
	glm::mat4 projection;
	glm::mat4 view;
	unsigned int vertexCount;
public:
	Renderer() = default;
	Renderer(const float* vertices, size_t size, unsigned int count);
	~Renderer();

	void setup(const float* vertices, size_t size);

	void clear() const {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void setCamera(const glm::mat4& viewMat, const glm::mat4& projMat);

	void draw();
};
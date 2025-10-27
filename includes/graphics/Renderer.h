#pragma once
#include <memory>
#include <glad/glad.h>

class Shader;

namespace vrtx {
	class VertexArray;
	class VertexBuffer;
}

class Renderer {
public:
	Renderer() = default;
	Renderer(const float* vertices, size_t size);
	~Renderer();

	void setup(const float* vertices, size_t size);


	void clear() const {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void draw(unsigned int vertexCount) const;

private:
	std::unique_ptr<vrtx::VertexArray> vertexArray;
	std::unique_ptr<vrtx::VertexBuffer> vertexBuffer;
	std::unique_ptr<Shader> shader;
};
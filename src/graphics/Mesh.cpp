#include "graphics/Mesh.h"

#include "graphics/Vertex.h"

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
	vbo = std::make_unique<vrtx::VertexBuffer>(vertices.data(), vertices.size() * sizeof(float));
	vao = std::make_unique<vrtx::VertexArray>();
	ibo = std::make_unique<vrtx::IndexBuffer>(indices.data(), indices.size());
	vao->bind();
	vbo->bind();
	ibo->bind();
}

void Mesh::draw() const {
}

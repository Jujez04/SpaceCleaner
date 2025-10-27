#include "graphics/Renderer.h"

#include <memory>

#include "graphics/MeshManager.h"
#include "graphics/Shader.h"
#include "graphics/Vertex.h"

/**
* Construttore del Renderer, in pratica inizializza tutto il processo di rendering
* tramite vertex buffer, vertex array e shader.
* @param vertices vertici da disegnare
* @param size dimensione dei vertici
*/
Renderer::Renderer(const float* vertices, size_t size) {
	setup(vertices, size);
	std::string vertexPath = "resources/vertex.glsl";
	std::string fragmentPath = "resources/fragment.glsl";
	shader = std::make_unique<Shader>(vertexPath, fragmentPath);
	vertexBuffer->unbind();
	vertexArray->unbind();
}

/**
* Distruttore di default
*/
Renderer::~Renderer() = default;

void Renderer::setup(const float* vertices, size_t size) {
	vertexArray = std::make_unique<vrtx::VertexArray>();
	vertexBuffer = std::make_unique<vrtx::VertexBuffer>(vertices, size);
	vertexArray->bind();
	vertexBuffer->bind();
	vertexBuffer->setData(vertices, size);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Renderer::draw(unsigned int vertexCount) const {
	shader->use();
	vertexArray->bind();
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}


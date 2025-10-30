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
Renderer::Renderer(const float* vertices, size_t size, unsigned int count)
	: vertexCount(count) {
	setup(vertices, size);
	std::string vertexPath = "../resources/vertex.glsl";
	std::string fragmentPath = "../resources/fragment.glsl";
	shader = std::make_unique<Shader>(vertexPath, fragmentPath);
}

/**
* Distruttore di default
*/
Renderer::~Renderer() = default;

void Renderer::setup(const float* vertices, size_t size) {
	vertexArray = std::make_unique<vrtx::VertexArray>();
	vertexBuffer = std::make_unique<vrtx::VertexBuffer>(vertices, size);

	vrtx::VertexBufferLayout bufferLayout;
	bufferLayout.push<float>(3);
	bufferLayout.push<float>(3);

	vertexArray->bind();
	vertexBuffer->bind();
	vertexArray->addBuffer(*vertexBuffer, bufferLayout);

	vertexBuffer->unbind();
	vertexArray->unbind();

}

void Renderer::setCamera(const glm::mat4& viewMat, const glm::mat4& projMat) {
	view = viewMat;
	projection = projMat;
}

void Renderer::draw() {
	shader->bind();
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);
	vertexArray->bind();
	glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
}


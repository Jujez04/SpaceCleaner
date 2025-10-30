#include "graphics/Mesh.h"

#include <memory>

#include "graphics/Vertex.h"
#include "graphics/Shader.h"
#include "graphics/Renderer.h"

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
    : vertices(vertices), indices(indices)
{
    setupMesh();
}

void Mesh::setupMesh() {
    va = std::make_unique<vrtx::VertexArray>();
    unsigned int vertexDataBytes = static_cast<unsigned int>(vertices.size() * sizeof(float));
    vb = std::make_unique<vrtx::VertexBuffer>(vertices.data(), vertexDataBytes);
    ib = std::make_unique<vrtx::IndexBuffer>(indices.data(), static_cast<unsigned int>(indices.size()));

    // layout: posizione (3 float) + colore (3 float) => stride = 6 * sizeof(float)
    vrtx::VertexBufferLayout layout;
    layout.push<float>(3); // position
    layout.push<float>(3); // color

    va->bind();
    vb->bind();
    va->addBuffer(*vb, layout);
    ib->bind();
    va->unbind();
    vb->unbind();
    ib->unbind();
}

void Mesh::setVertices(const std::vector<float>& verts) {
    vertices = verts;
    setupMesh(); 
}

void Mesh::setIndices(const std::vector<unsigned int>& inds) {
    indices = inds;
    setupMesh();
}

void Mesh::draw(Shader& shader, GLenum usage) const {
    if (!va || !ib) return;
    shader.bind();
    va->bind();
    ib->bind();
    glDrawElements(usage, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
    ib->unbind();
    va->unbind();
    shader.unbind();
}

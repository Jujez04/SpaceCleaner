#include "graphics/Mesh.h"
#include "graphics/Shader.h"
#include "graphics/Renderer.h"

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
    : vertices(vertices), indices(indices)
{
    setupMesh();
}

void Mesh::setupMesh() {
    // Crea i buffer principali (VAO, VBO, IBO)
    va = std::make_unique<vrtx::VertexArray>();
    unsigned int vertexDataBytes = static_cast<unsigned int>(vertices.size() * sizeof(float));
    vb = std::make_unique<vrtx::VertexBuffer>(vertices.data(), vertexDataBytes);
    ib = std::make_unique<vrtx::IndexBuffer>(indices.data(), static_cast<unsigned int>(indices.size()));

    // Layout dei vertici: solo posizione (3 float per vertice)
    vrtx::VertexBufferLayout layout;
    layout.push<float>(3); // posizione: x, y, z

    // Collega buffer e layout all'array dei vertici
    va->bind();
    vb->bind();
    va->addBuffer(*vb, layout);
    ib->bind();

    // Pulizia
    va->unbind();
    vb->unbind();
    ib->unbind();
}

void Mesh::setVertices(const std::vector<float>& verts) {
    vertices = verts;
    setupMesh(); // Ricrea i buffer
}

void Mesh::setIndices(const std::vector<unsigned int>& inds) {
    indices = inds;
    setupMesh(); // Ricrea i buffer
}

void Mesh::draw(Shader& shader, GLenum usage) const {
    if (!va || !ib) return; // Se la mesh non è inizializzata, non disegnare

    va->bind();
    ib->bind();

    // Esegue il disegno con gli indici
    glDrawElements(usage, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);

    ib->unbind();
    va->unbind();
}

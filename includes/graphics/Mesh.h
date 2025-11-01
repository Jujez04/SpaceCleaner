#pragma once
#include <vector>
#include <memory>
#include "graphics/Vertex.h"

class Shader;
namespace vrtx {
    class VertexArray;
    class VertexBuffer;
    class IndexBuffer;
    class VertexBufferLayout;
}

class Mesh {
private:
    unsigned int meshId;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    std::unique_ptr<vrtx::VertexArray> va;
    std::unique_ptr<vrtx::VertexBuffer> vb;
    std::unique_ptr<vrtx::IndexBuffer> ib;

    void setupMesh();

public:
    Mesh() = default;
    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    ~Mesh() = default;

    void setVertices(const std::vector<float>& vertices);
    void setIndices(const std::vector<unsigned int>& indices);

    void draw(Shader& shader, GLenum usage) const;

    unsigned int getId() const { return meshId; }
    void setId(const unsigned int id) { meshId = id; }

};

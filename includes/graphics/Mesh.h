#pragma once


#include <vector>
#include <memory>

namespace vrtx {
    class VertexArray;
    class VertexBuffer;
    class IndexBuffer;
}

class Mesh {
public:
    Mesh(const std::vector<float>& vertices,
        const std::vector<unsigned int>& indices);
    void draw() const;
private:
    std::unique_ptr<vrtx::VertexArray> vao;
    std::unique_ptr<vrtx::VertexBuffer> vbo;
    std::unique_ptr<vrtx::IndexBuffer> ibo;
};

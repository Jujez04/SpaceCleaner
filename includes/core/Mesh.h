#pragma once

#ifndef MESH_H
#define MESH_H

#include <vector>
#include <memory>

class VertexArray;
class VertexBuffer;
class IndexBuffer;

class Mesh {
public:
    Mesh(const std::vector<float>& vertices,
        const std::vector<unsigned int>& indices);
    void Draw() const;
private:
    std::unique_ptr<VertexArray> vao;
    std::unique_ptr<VertexBuffer> vbo;
    std::unique_ptr<IndexBuffer> ibo;
};


#endif
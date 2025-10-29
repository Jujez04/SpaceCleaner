#pragma once

class Vertex;

/**
 * Classe Mesh che rappresenta la geometria di una figura
 * Gestisce vertici, indici e rendering
 */
class Mesh {
public:
    // Costruttori
    Mesh();
    ~Mesh();

    // Disabilita copia, abilita move
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    // Metodi principali
    void setVertices(const std::vector<Vertex>& vertices);
    void setIndices(const std::vector<unsigned int>& indices);
    void updateVertices(const std::vector<Vertex>& vertices);

    // Rendering
    void draw() const;
    void drawInstanced(unsigned int instanceCount) const;

    // Getters
    const std::vector<Vertex>& getVertices() const { return vertices; }
    const std::vector<unsigned int>& getIndices() const { return indices; }
    unsigned int getVertexCount() const { return vertices.size(); }
    unsigned int getIndexCount() const { return indices.size(); }

    static Mesh create();

private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;

    bool isSetup = false;

    // Setup methods
    void setupMesh();
    void cleanup();
    void calculateNormals();
};

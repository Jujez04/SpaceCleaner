#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "graphics/Vertex.h"

class Shader;

namespace vrtx {
    class VertexArray;
    class VertexBuffer;
    class IndexBuffer;
    class VertexBufferLayout;
}

/**
 * @brief Rappresenta una mesh 2D o 3D composta da vertici e indici.
 *
 * La classe si occupa della gestione dei buffer OpenGL (VAO, VBO, EBO)
 * e della configurazione del layout dei vertici. È responsabile della
 * preparazione dei dati per il rendering.
 */
class Mesh {
private:
    unsigned int meshId; ///< Identificatore univoco della mesh (gestito dall’engine)
    std::vector<float> vertices; ///< Dati dei vertici (posizioni, colori, ecc.)
    std::vector<unsigned int> indices; ///< Indici dei vertici per la triangolazione

    /// Punti estremi dell’Axis-Aligned Bounding Box (AABB)
    glm::vec2 minPoint = glm::vec2(0.0f);
    glm::vec2 maxPoint = glm::vec2(0.0f);

    /// Buffer OpenGL per la rappresentazione della mesh
    std::unique_ptr<vrtx::VertexArray> va;
    std::unique_ptr<vrtx::VertexBuffer> vb;
    std::unique_ptr<vrtx::IndexBuffer> ib;

    /**
     * @brief Configura i buffer OpenGL (VAO, VBO, IBO) e il layout dei vertici.
     *
     * Metodo interno chiamato automaticamente dal costruttore o dai setter
     * per inizializzare la struttura grafica della mesh.
     */
    void setupMesh();

public:
    /// Costruttore di default: crea una mesh vuota
    Mesh() = default;

    /**
     * @brief Costruttore che inizializza la mesh con vertici e indici.
     * @param vertices Dati dei vertici (posizioni, colori, ecc.)
     * @param indices Indici che definiscono la topologia della mesh
     */
    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);

    /// Distruttore di default (i buffer vengono gestiti automaticamente da unique_ptr)
    ~Mesh() = default;

    /// Imposta i vertici della mesh e ricostruisce i buffer
    void setVertices(const std::vector<float>& vertices);

    /// Imposta gli indici della mesh e ricostruisce i buffer
    void setIndices(const std::vector<unsigned int>& indices);

    /// Restituisce il vettore dei vertici
    std::vector<float> getVertices() const { return vertices; }

    /**
     * @brief Esegue il rendering della mesh.
     * @param shader Shader da utilizzare per il disegno
     * @param usage Modalità di disegno OpenGL (es. GL_TRIANGLES)
     */
    void draw(Shader& shader, GLenum usage) const;

    /// Restituisce l’ID della mesh
    unsigned int getId() const { return meshId; }

    /// Imposta l’ID della mesh (ad esempio assegnato da un gestore di risorse)
    void setId(const unsigned int id) { meshId = id; }

    /// Imposta il punto minimo dell’AABB
    void setMin(glm::vec2 vec) { minPoint = vec; }

    /// Imposta il punto massimo dell’AABB
    void setMax(glm::vec2 vec) { maxPoint = vec; }
};

#pragma once
#include <memory>
#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Mesh;
class MeshManager;
class ShaderManager;
class Shader;
class Entity;
struct AABB;

/**
 * @brief Gestisce il rendering di mesh ed entità nel motore grafico.
 *
 * Contiene lo stato della telecamera (matrici view/projection)
 * e le funzioni per disegnare oggetti tramite ID o componenti di rendering.
 */
class Renderer {
private:
    std::vector<std::shared_ptr<Mesh>> meshes; ///< Cache locale delle mesh da disegnare
    glm::mat4 projection; ///< Matrice di proiezione corrente
    glm::mat4 view; ///< Matrice di vista corrente

public:
    Renderer();
    ~Renderer() = default;

    /// Aggiunge una mesh alla lista di rendering
    void addMesh(const std::shared_ptr<Mesh>& mesh);

    /// Pulisce la scena corrente
    void clear();

    /// Imposta le matrici di vista e proiezione (camera)
    void setCamera(const glm::mat4& viewMat, const glm::mat4& projMat);

    /**
     * @brief Disegna una mesh tramite ID e shader specifico.
     *
     * @param meshId ID della mesh registrata
     * @param shaderId ID dello shader registrato
     * @param color Colore del disegno
     * @param model Matrice di trasformazione modello
     * @param mode Modalità di disegno (es. GL_TRIANGLES)
     */
    void drawMesh(const unsigned int meshId, const unsigned int shaderId, const glm::vec4& color, const glm::mat4& model, const GLenum mode);

    /**
     * @brief Disegna un’entità completa (con submesh e shader multipli).
     *
     * @param entity Entità da disegnare
     * @param mode Modalità di disegno (default GL_TRIANGLES)
     */
    void drawEntityByInfo(Entity& entity, const GLenum mode = GL_TRIANGLES);

    /// Disegna una bounding box 2D per il debug
    void drawBoundingBox(const AABB& box, const glm::vec4& color, const glm::mat4& view, const glm::mat4& projection);
};

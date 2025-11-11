/**
 * @file Renderer.cpp
 * @brief Implementazione della classe Renderer, responsabile del disegno delle entità e delle mesh.
 */

#include "graphics/Renderer.h"
#include "graphics/MeshManager.h"
#include "graphics/Shader.h"
#include "graphics/Vertex.h"
#include "graphics/Mesh.h"
#include "game/SpaceCleaner.h"
#include "graphics/ShaderManager.h"
#include "game/GameObject.h"
#include "game/Collision.h"
#include "utilities/Timer.h"

 /**
  * @brief Costruttore del Renderer. Inizializza la telecamera con matrici di identità e default.
  */
Renderer::Renderer() {
    this->setCamera(glm::mat4(1.0f), glm::mat4(0.0f));
}

/**
 * @brief Aggiunge una mesh alla lista delle mesh gestite dal renderer.
 * @param mesh Puntatore condiviso alla mesh da aggiungere.
 */
void Renderer::addMesh(const std::shared_ptr<Mesh>& mesh) {
    meshes.push_back(mesh);
}

/**
 * @brief Imposta le matrici di vista e proiezione della telecamera.
 * @param viewMat Matrice di vista.
 * @param projMat Matrice di proiezione.
 */
void Renderer::setCamera(const glm::mat4& viewMat, const glm::mat4& projMat) {
    view = viewMat;
    projection = projMat;
}

/**
 * @brief Disegna una singola mesh utilizzando uno shader specificato.
 * @param meshId ID della mesh da disegnare.
 * @param shaderId ID dello shader da utilizzare.
 * @param color Colore da applicare come uniform.
 * @param model Matrice modello (trasformazione dell'oggetto).
 * @param mode Modalità di disegno (es. GL_TRIANGLES, GL_LINES, ecc.).
 */
void Renderer::drawMesh(const unsigned int meshId, const unsigned int shaderId, const glm::vec4& color, const glm::mat4& model, const GLenum mode)
{
    std::shared_ptr<Shader> shader = ShaderManager::get(shaderId);
    if (!shader) return;

    shader->bind();
    shader->setUniformMat4("view", view);
    shader->setUniformMat4("projection", projection);
    shader->setUniformMat4("model", model);
    shader->setUniformVec4("uColor", color);
    shader->setUniform1f("uTime", static_cast<float>(Timer::totalTime));

    auto mesh = MeshManager::getById(meshId);
    if (mesh) {
        mesh->draw(*shader, mode);
    }

    shader->unbind();
}

/**
 * @brief Pulisce lo schermo impostando il colore di sfondo e resettando i buffer di colore e profondità.
 */
void Renderer::clear() {
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * @brief Disegna un'entità composta da più submesh, ciascuna con shader e trasformazione propri.
 * @param entity Entità da disegnare.
 * @param mode Modalità di disegno (default GL_TRIANGLES).
 */
void Renderer::drawEntityByInfo(Entity& entity, const GLenum mode) {
    if (!entity.isActive() || !entity.renderData.enabled) return;

    unsigned int currentShaderId = 0;
    std::shared_ptr<Shader> activeShader = nullptr;

    const glm::mat4 entityModel = entity.transform.getModelMatrix();

    for (const auto& subMesh : entity.renderData.getSubMeshes()) {
        if (subMesh.shaderId != currentShaderId && subMesh.visible) {
            if (activeShader) activeShader->unbind();
            activeShader = ShaderManager::get(subMesh.shaderId);

            if (activeShader) {
                activeShader->bind();
                currentShaderId = subMesh.shaderId;
                activeShader->setUniformMat4("view", view);
                activeShader->setUniformMat4("projection", projection);
            }
        }

        if (activeShader) {
            glm::mat4 finalModel = entityModel * subMesh.localTransform;
            activeShader->setUniformMat4("model", finalModel);
            activeShader->setUniformVec4("uColor", subMesh.color);

            auto mesh = MeshManager::getById(subMesh.meshId);
            if (mesh) mesh->draw(*activeShader, mode);
        }
    }

    if (activeShader) activeShader->unbind();
}

/**
 * @brief Disegna un riquadro (bounding box) 2D per il debug delle collisioni.
 * @param box Oggetto AABB contenente i punti minimo e massimo.
 * @param color Colore del contorno.
 * @param view Matrice di vista corrente.
 * @param projection Matrice di proiezione corrente.
 */
void Renderer::drawBoundingBox(const AABB& box, const glm::vec4& color, const glm::mat4& view, const glm::mat4& projection)
{
    float vertices[] = {
        box.min.x, box.min.y, 0.0f,
        box.max.x, box.min.y, 0.0f,
        box.max.x, box.max.y, 0.0f,
        box.min.x, box.max.y, 0.0f
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    auto shader = ShaderManager::get("DefaultShader");
    if (shader) {
        shader->bind();
        shader->setUniformMat4("view", view);
        shader->setUniformMat4("projection", projection);
        shader->setUniformMat4("model", glm::mat4(1.0f));
        shader->setUniformVec4("uColor", color);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        shader->unbind();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

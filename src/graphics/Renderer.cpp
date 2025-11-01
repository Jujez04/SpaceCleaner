#include "graphics/Renderer.h"

#include "graphics/MeshManager.h"
#include "graphics/Shader.h"
#include "graphics/Vertex.h"
#include "graphics/Mesh.h"
#include "game/SpaceCleaner.h"
#include "graphics/ShaderManager.h"
#include "game/GameObject.h"

Renderer::Renderer(const std::string& vertexPath, const std::string& fragmentPath) {
    shader = std::make_unique<Shader>(vertexPath, fragmentPath);
    this->setCamera(glm::mat4(1.0f), glm::mat4(0.0f));
}

Renderer::~Renderer() = default;

void Renderer::addMesh(const std::shared_ptr<Mesh>& mesh) {
    meshes.push_back(mesh);
}

void Renderer::setCamera(const glm::mat4& viewMat, const glm::mat4& projMat) {
    view = viewMat;
    projection = projMat;
}

void Renderer::clear() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::drawAll(GLenum mode) {
    if (!shader) return;

    shader->bind();
    shader->setUniformMat4("view", view);
    shader->setUniformMat4("projection", projection);

    for (const auto& mesh : meshes) {
        if (mesh)
            mesh->draw(*shader, mode);
    }

    shader->unbind();
}

void Renderer::drawEntity(Entity& entity, GLenum mode) {
    if (!shader) return;
    if (!entity.getMeshComp().isVisible()) return;

    shader->bind();
    shader->setUniformMat4("view", view);
    shader->setUniformMat4("projection", projection);

    glm::mat4 model = entity.transform.getModelMatrix();
    shader->setUniformMat4("model", model);
    shader->setUniformVec4("color", entity.getColorComp().getColor());

    auto mesh = MeshManager::get(entity.getMeshComp().getMeshName());
    if (mesh)
        mesh->draw(*shader, mode);

    shader->unbind();
}


void Renderer::drawEntityByInfo(Entity& entity, GLenum mode) {
    if (!entity.isActive() || !entity.renderData.enabled) return;

    // 2. Ottieni la matrice di trasformazione GLOBALE dell'entità
    // La matrice è la stessa per tutte le sub-mesh.
    glm::mat4 entityModel = entity.transform.getModelMatrix();

    // 3. Itera su TUTTE le sub-mesh da disegnare
    for (const auto& subMesh : entity.renderData.getSubMeshes()) {
        if (!subMesh.visible) continue;

        // --- Per ogni Sub-Mesh, esegui un draw call completo ---

        // 4. Seleziona e Binda lo Shader CORRETTO per questa sub-mesh
        // NOTA: Per ora usiamo lo shader del Renderer (shader), ma in un sistema completo
        // dovresti usare ShaderManager::get(subMesh.shaderId) per ottenere lo shader corretto.

        // Supponiamo che tutti usino lo shader base del Renderer per il momento
        if (!shader) continue;
        shader->bind();

        // 5. Imposta le matrici View e Projection (solo se cambiano, altrimenti meglio fuori dal ciclo)
        shader->setUniformMat4("view", view);
        shader->setUniformMat4("projection", projection);

        // 6. Calcola la matrice MODEL finale (Globale * Locale)
        glm::mat4 finalModel = entityModel * subMesh.localTransform;
        shader->setUniformMat4("model", finalModel);

        // 7. Imposta il COLORE specifico per questa sub-mesh
        shader->setUniformVec4("color", subMesh.color);

        // 8. Ottieni la Mesh e disegna
        // Usiamo l'ID, assumendo che MeshManager::get() possa usare anche l'ID
        auto mesh = MeshManager::getById(subMesh.meshId);
        if (mesh) {
            mesh->draw(*shader, mode);
        }

        shader->unbind();
    }
}

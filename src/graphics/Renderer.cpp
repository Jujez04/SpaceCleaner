#include "graphics/Renderer.h"

#include "graphics/MeshManager.h"
#include "graphics/Shader.h"
#include "graphics/Vertex.h"
#include "graphics/Mesh.h"
#include "game/SpaceCleaner.h"
#include "graphics/ShaderManager.h"
#include "game/GameObject.h"
#include "utilities/Timer.h"

Renderer::Renderer() {
    this->setCamera(glm::mat4(1.0f), glm::mat4(0.0f));
}

void Renderer::addMesh(const std::shared_ptr<Mesh>& mesh) {
    meshes.push_back(mesh);
}

void Renderer::setCamera(const glm::mat4& viewMat, const glm::mat4& projMat) {
    view = viewMat;
    projection = projMat;
}

void Renderer::drawMesh(unsigned int meshId, unsigned int shaderId, const glm::vec4& color, const glm::mat4& model, GLenum mode)
{
    std::shared_ptr<Shader> shader = ShaderManager::get(shaderId);
    if (!shader) return;

    shader->bind();

    // 2. Imposta le Uniformi (Usa le matrici della telecamera già caricate)
    shader->setUniformMat4("view", view);
    shader->setUniformMat4("projection", projection);

    // 3. Imposta la Matrice Modello fornita (Questa è la trasformazione del Cuore)
    shader->setUniformMat4("model", model);
    shader->setUniformVec4("uColor", color);
    shader->setUniform1f("uTime", Timer::totalTime);

    // 4. Ottieni e Disegna la Mesh
    auto mesh = MeshManager::getById(meshId);
    if (mesh) {
        mesh->draw(*shader, mode);
    }

    // 5. Unbind
    shader->unbind();
}

void Renderer::clear() {
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/*
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
*/

void Renderer::drawEntityByInfo(Entity& entity, GLenum mode) {
    if (!entity.isActive() || !entity.renderData.enabled) return;

    unsigned int currentShaderId = 0;
    std::shared_ptr<Shader> activeShader = nullptr;

    const glm::mat4 entityModel = entity.transform.getModelMatrix();

    for (const auto& subMesh : entity.renderData.getSubMeshes()) {
        if (subMesh.shaderId != currentShaderId && subMesh.visible) {

            if (activeShader) {
                activeShader->unbind();
            }
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
            if (mesh) {
                mesh->draw(*activeShader, mode);
            }
        }
    }

    if (activeShader) {
        activeShader->unbind();
    }
}
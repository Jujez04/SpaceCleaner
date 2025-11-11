#include "game/GameObject.h"
#include <glm/gtc/matrix_transform.hpp>
#include "game/Collision.h"
#include "graphics/Mesh.h"
#include "graphics/MeshManager.h"

int Entity::nextId = 0;

/**
 * @brief Calcola la matrice modello combinando traslazione, rotazione e scala.
 *
 * Usa le funzioni GLM (compatibili con OpenGL) per costruire una Model Matrix:
 *  - `translate` sposta l'oggetto nello spazio;
 *  - `rotate` applica una rotazione sul piano Z;
 *  - `scale` ridimensiona la mesh.
 */
const glm::mat4 TransformComponent::getModelMatrix() const {
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(scale, 1.0f));
    return model;
}

// Metodi di modifica del transform (autoesplicativi)
void TransformComponent::setPosition(const glm::vec2& pos) { position = pos; }
void TransformComponent::setRotation(float rot) { rotation = rot; }
void TransformComponent::setScale(const glm::vec2& scl) { scale = scl; }
void TransformComponent::translate(const glm::vec2& offset) { position += offset; }
void TransformComponent::rotate(float angle) { rotation += angle; }
void TransformComponent::scaleBy(const glm::vec2& factor) { scale *= factor; }

void RenderComponent::addSubMesh(SubMeshRenderInfo& meshInfo) {
    subMeshes.push_back(meshInfo);
}

const std::vector<SubMeshRenderInfo>& RenderComponent::getSubMeshes() const {
    return subMeshes;
}

void RenderComponent::clearSubMesh() {
    subMeshes.clear();
}

// ---------- Entity: constructor e gestione layers ----------
Entity::Entity(const std::string& entityName)
    : id(nextId++), name(entityName), active(true), transform(), renderData() {
    // Costruttore: assegna un id univoco, nome e abilita l'entità.
    // I componenti (transform, renderData) vengono inizializzati con i loro costruttori di default.
}

void Entity::addMeshLayer(SubMeshRenderInfo meshInfo) {
    renderData.addSubMesh(meshInfo);
}

void Entity::clearMeshLayers() {
    renderData.clearSubMesh();
}

/*
 * @brief Restituisce l'AABB dell'entità combinando tutte le sub-mesh.
 *
 * Strategia:
 *  - Per ogni SubMesh presente in renderData, otteniamo la Mesh dal MeshManager.
 *  - Calcoliamo l'AABB della mesh applicando la trasformazione globale dell'entità
 *    moltiplicata per la trasformazione locale della sub-mesh.
 *  - Accumuliamo min/max per ottenere l'AABB complessiva dell'entità.
 *
 * Nota: dipende da calculateMeshAABB(mesh, transformMatrix) definita in game/Collision.h.
 */
AABB Entity::getAABB() const {
    // Inizializziamo un AABB "vuoto" che si espanderà via via.
    AABB result;

    // Se non ci sono submesh, ritorniamo un AABB vuoto (min > max)
    const auto& subs = renderData.getSubMeshes();
    if (subs.empty()) {
        return result;
    }

    // Trasformazione globale dell'entità (model matrix)
    glm::mat4 globalModel = transform.getModelMatrix();

    bool first = true;
    for (const auto& sub : subs) {
        // Ottieni la mesh dal MeshManager
        auto meshPtr = MeshManager::getById(sub.meshId);
        if (meshPtr) {
            // Componiamo la trasformazione: globale * locale della submesh
            glm::mat4 composed = globalModel * sub.localTransform;

            // Calcoliamo l'AABB per questa mesh trasformata
            AABB meshAabb = calculateMeshAABB(meshPtr, composed);

            if (first) {
                result = meshAabb;
                first = false;
            }
            else {
                // Espandiamo il risultato con i nuovi min/max
                result.min.x = std::min(result.min.x, meshAabb.min.x);
                result.min.y = std::min(result.min.y, meshAabb.min.y);
                result.max.x = std::max(result.max.x, meshAabb.max.x);
                result.max.y = std::max(result.max.y, meshAabb.max.y);
            }
        }

        
    }

    return result;
}
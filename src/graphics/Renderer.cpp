#include "graphics/Renderer.h"

#include "graphics/MeshManager.h"
#include "graphics/Shader.h"
#include "graphics/Vertex.h"
#include "graphics/Mesh.h"
#include "game/SpaceCleaner.h"
#include "graphics/ShaderManager.h"

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

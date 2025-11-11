#include "graphics/MeshManager.h"
#include "graphics/Mesh.h"

std::unordered_map<std::string, std::shared_ptr<Mesh>> MeshManager::meshMap;
std::unordered_map<unsigned int, std::shared_ptr<Mesh>> MeshManager::idMeshMap;
unsigned int MeshManager::nextId = 1;

std::shared_ptr<Mesh> MeshManager::registerMesh(
    const std::string& name,
    const std::vector<float>& vertices,
    const std::vector<unsigned int>& indices
) {
    // Se esiste già una mesh con questo nome, restituiscila
    if (meshMap.count(name)) {
        return meshMap[name];
    }

    // Crea una nuova mesh e registrala con un ID univoco
    auto mesh = std::make_shared<Mesh>(vertices, indices);
    unsigned int newId = nextId++;
    mesh->setId(newId);

    idMeshMap[newId] = mesh;
    meshMap[name] = mesh;

    return mesh;
}

std::shared_ptr<Mesh> MeshManager::registerMeshById(
    const std::vector<float>& vertices,
    const std::vector<unsigned int>& indices
) {
    auto mesh = std::make_shared<Mesh>(vertices, indices);
    idMeshMap[nextId] = mesh;
    mesh->setId(nextId);
    nextId++;
    return mesh;
}

std::shared_ptr<Mesh> MeshManager::get(const std::string& name) {
    auto it = meshMap.find(name);
    return it != meshMap.end() ? it->second : nullptr;
}

std::shared_ptr<Mesh> MeshManager::getById(const unsigned int id) {
    auto it = idMeshMap.find(id);
    return it != idMeshMap.end() ? it->second : nullptr;
}

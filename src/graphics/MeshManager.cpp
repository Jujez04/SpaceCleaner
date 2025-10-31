#include "graphics/MeshManager.h"

#include "graphics/Mesh.h"

std::shared_ptr<Mesh> MeshManager::registerMesh(const std::string& name, const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
{
    auto mesh = std::make_shared<Mesh>(vertices, indices);
    meshMap[name] = mesh;
    return mesh;
}

std::shared_ptr<Mesh> MeshManager::get(const std::string& name) {
    auto it = meshMap.find(name);
    return it != meshMap.end() ? it->second : nullptr;
}
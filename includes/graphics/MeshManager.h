#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Mesh;

class MeshManager {
public:
	static std::shared_ptr<Mesh> registerMesh(const std::string& name,
		const std::vector<float>& vertices,
		const std::vector<unsigned int>& indices = {});
	static std::shared_ptr<Mesh> registerMeshById(
		const std::vector<float>& vertices,
		const std::vector<unsigned int>& indices = {});
	static std::shared_ptr<Mesh> get(const std::string& name);
	static std::shared_ptr<Mesh> getById(unsigned int id);

private:
	static std::unordered_map<std::string, std::shared_ptr<Mesh>> meshMap;
	static std::unordered_map<unsigned int, std::shared_ptr<Mesh>> idMeshMap;
	static unsigned int nextId;
};

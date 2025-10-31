#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Mesh;

class MeshManager {
public:
	MeshManager();
	~MeshManager();

	std::shared_ptr<Mesh> registerMesh(const std::string& name,
		const std::vector<float>& vertices,
		const std::vector<unsigned int>& indices = {});

	std::shared_ptr<Mesh> get(const std::string& name);

private:
	std::unordered_map<std::string, std::shared_ptr<Mesh>> meshMap;
};

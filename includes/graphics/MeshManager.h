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

	static std::shared_ptr<Mesh> get(const std::string& name);

private:
	static std::unordered_map<std::string, std::shared_ptr<Mesh>> meshMap;
};

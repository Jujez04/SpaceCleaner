#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

class HermiteMesh {
public:

	static unsigned int baseHermiteToMesh(const std::string& name, const std::vector<glm::vec2>& controlPoints, const int segmentsPerCurve = 40);
};
#include "math/Hermite.h"

#include <cmath>


float HermiteUtility::hP0(float t) {
	return 2 * t * t * t - 3 * pow(t, 2) + 1;
}

float HermiteUtility::hP1(float t) {
	return -2 * t * t * t + 3 * t * t;
}

float HermiteUtility::hT0(float t) {
	return pow(t, 3) - 2 * pow(t, 2) + t;
}

float HermiteUtility::hT1(float t) {
	return pow(t, 3) - pow(t, 2);
}

glm::vec2 HermiteUtility::hermite(float t, const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& t0, const glm::vec2& t1)
{
	return hP0(t) * p0 + hP1(t) * p1 + hT0(t) * t0 + hT1(t) * t1;
}

std::vector<glm::vec2> HermiteUtility::generateHermiteCurve(glm::vec2 p0, glm::vec2 p1, glm::vec2 t0, glm::vec2 t1, int segments)
{
	std::vector<glm::vec2> curvePoints;
	for (int i = 0; i <= segments; ++i) {
		float t = static_cast<float>(i) / segments;
		curvePoints.push_back(hermite(t, p0, p1, t0, t1));
	}
	return curvePoints;
}

std::vector<glm::vec2> HermiteUtility::generateClosedHermiteCurve(const std::vector<glm::vec2>& controlPoints, const std::vector<glm::vec2>& tangents, int segmentsPerCurve)
{
	int n = controlPoints.size();
	std::vector<glm::vec2> closedCurve;
	for (int i = 0; i < n; i++) {	
		glm::vec2 p0 = controlPoints[i];
		glm::vec2 p1 = controlPoints[(i + 1) % n];
		glm::vec2 t0 = tangents[i];
		glm::vec2 t1 = tangents[(i + 1) % n];
		std::vector<glm::vec2> curveSegment = generateHermiteCurve(p0, p1, t0, t1, segmentsPerCurve);
		closedCurve.insert(closedCurve.end(), curveSegment.begin(), curveSegment.end() - 1);
	}
	return closedCurve;
}

std::vector<glm::vec2> HermiteUtility::calculateTangents(const std::vector<glm::vec2>& controlPoints, float tension)
{
	std::vector<glm::vec2> tangents;
	int n = controlPoints.size();
	for (int i = 0; i < n; i++) {
		glm::vec2 pPrev = controlPoints[(i - 1 + n) % n];
		glm::vec2 pNext = controlPoints[(i + 1) % n];
		glm::vec2 tangent = (1.0f - tension) * 0.5f * (pNext - pPrev);
		tangents.push_back(tangent);
	}
	return tangents;
}

#include "math/CatmullRom.h"

#include "math/Hermite.h"

glm::vec2 SplineUtility::kochanekBartelsTangent(
    const glm::vec2& prev,
    const glm::vec2& curr,
    const glm::vec2& next,
    float tension, float bias, float continuity,
    bool isOutgoing
) {
    float mul = (1.0f - tension) * 0.5f;
    if (isOutgoing) {
        return mul * ((1 + bias) * (1 + continuity) * (curr - prev) +
            (1 - bias) * (1 - continuity) * (next - curr));
    }
    else {
        return mul * ((1 + bias) * (1 - continuity) * (curr - prev) +
            (1 - bias) * (1 + continuity) * (next - curr));
    }
}

// Cambia il nome Catmull Rom in SplineCurveTBC e rimuovi ogni riferimento a CatmullRom
std::vector<glm::vec2> SplineUtility::generateTBC(
    const std::vector<glm::vec2>& controlPoints,
    const std::vector<TBCParams>& params,
    int segmentsPerCurve
) {
    std::vector<glm::vec2> curvePoints;
    int n = controlPoints.size();

    for (int i = 0; i < n; ++i) {
        glm::vec2 p0 = controlPoints[(i - 1 + n) % n];
        glm::vec2 p1 = controlPoints[i];
        glm::vec2 p2 = controlPoints[(i + 1) % n];
        glm::vec2 p3 = controlPoints[(i + 2) % n];

        const auto& par1 = params[i];
        const auto& par2 = params[(i + 1) % n];

        glm::vec2 t1 = kochanekBartelsTangent(p0, p1, p2, par1.tension, par1.bias, par1.continuity, true);
        glm::vec2 t2 = kochanekBartelsTangent(p1, p2, p3, par2.tension, par2.bias, par2.continuity, false);

        auto segment = HermiteUtility::generateHermiteCurve(p1, p2, t1, t2, segmentsPerCurve);
        curvePoints.insert(curvePoints.end(), segment.begin(), segment.end() - 1);
    }
    return curvePoints;
}
#pragma once
#include <glm/glm.hpp>
#include <vector>

struct TBCParams {
    float tension;
    float bias;
    float continuity;
};

class SplineUtility {
public:
    static glm::vec2 kochanekBartelsTangent(
        const glm::vec2& prev,
        const glm::vec2& curr,
        const glm::vec2& next,
        float tension, float bias, float continuity,
        bool isOutgoing
    );

    static std::vector<glm::vec2> generateTBC(
        const std::vector<glm::vec2>& controlPoints,
        const std::vector<TBCParams>& params,
        int segmentsPerCurve
    );
};

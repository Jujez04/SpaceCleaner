#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(float screenWidth, float screenHeight);

    void setPosition(const glm::vec2& pos);
    //void move(const glm::vec2& delta);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    const glm::vec2& GetPosition() const { return position; }

private:
    glm::vec2 position;
    float width;
    float height;
};

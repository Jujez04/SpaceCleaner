#pragma once
#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Forward declarations
class Entity;

class Component {
public:
    virtual ~Component() = default;

    bool enabled = true;
    Entity* owner = nullptr;
};

class TransformComponent : public Component {
private:
    glm::vec2 position;
    glm::vec2 scale;
    float rotation;

public:
    TransformComponent(
        const glm::vec2& pos = glm::vec2(0.0f),
        float rot = 0.0f,
        const glm::vec2& scl = glm::vec2(1.0f)
    ) : position(pos), rotation(rot), scale(scl) {
    }

    ~TransformComponent() override = default;

    // Model matrix con caching
    const glm::mat4 getModelMatrix() const;

    // Getters
    const glm::vec2& getPosition() const { return position; }
    float getRotation() const { return rotation; }
    const glm::vec2& getScale() const { return scale; }

    // Setters con dirty flag
    void setPosition(const glm::vec2& pos);
    void setRotation(float rot);
    void setScale(const glm::vec2& scl);

    // Utility methods
    void translate(const glm::vec2& offset);
    void rotate(float angle);
    void scaleBy(const glm::vec2& factor);

};

class MeshComponent : public Component {
private:
    std::string meshName;
    unsigned int meshId;
    unsigned int shaderId;
    bool visible;

public:
    MeshComponent() : meshId(0), shaderId(0), visible(true) {}
    MeshComponent(unsigned int meshId, unsigned int shaderId) : meshId(meshId), shaderId(shaderId), visible(true) {}
    ~MeshComponent() override = default;

    // Visibility
    bool isVisible() const { return visible && enabled; }
    void setVisible(bool vis) { visible = vis; }

    // Getter e Setter
    unsigned int getMeshId() const { return meshId; }
    unsigned int getShaderId() const { return shaderId; }
    void setMeshId(unsigned int id) { meshId = id; }
    void setShaderId(unsigned int id) { shaderId = id; }
    const std::string& getMeshName() const { return meshName; }
    void setMeshName(const std::string& name) { meshName = name; }
};

class ColorComponent : public Component {
private:
    glm::vec4 color;

public:
    ColorComponent() : color(1.0f) {}
    explicit ColorComponent(const glm::vec4& col) : color(col) {}
    explicit ColorComponent(const glm::vec3& col, float alpha = 1.0f)
        : color(col, alpha) {
    }
    ~ColorComponent() override = default;

    const glm::vec4& getColor() const { return color; }
    void setColor(const glm::vec4& col) { color = col; }
    void setColor(const glm::vec3& col, float alpha = 1.0f) {
        color = glm::vec4(col, alpha);
    }

    float getAlpha() const { return color.a; }
    void setAlpha(float alpha) { color.a = alpha; }
};

class Entity {
private:
    static int nextId;

    int id;
    std::string name;
    bool active;

public:
    // Components come membri pubblici (stile composition-based)
    TransformComponent transform;
    MeshComponent mesh;
    ColorComponent color;

    Entity(const std::string& entityName = "Entity");
    virtual ~Entity() = default;

    // Prevent copying (use shared_ptr per condivisione)
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

    // Allow moving
    Entity(Entity&&) = default;
    Entity& operator=(Entity&&) = default;

    // Core methods
    virtual void update(float deltaTime) = 0;
    virtual void onCollision(Entity* other) {}

    // Getters
    int getId() const { return id; }
    const std::string& getName() const { return name; }
    bool isActive() const { return active; }
    MeshComponent getMeshComp() const { return mesh; }
    TransformComponent getTransform() const { return transform; }
    ColorComponent getColorComp() const { return color; }

    // State management
    void setActive(bool state) { active = state; }
    void setName(const std::string& newName) { name = newName; }
};
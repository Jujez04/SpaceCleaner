#pragma once

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Component {};

class TransformComponent : public Component {
private:
	glm::vec2 position;
	float rotation;
	glm::vec2 scale;
public:
	~TransformComponent();
	TransformComponent(glm::vec2 pos = glm::vec2(0.0f),
		float rot = 0.0f,
		glm::vec2 scl = glm::vec2(1.0f))
		: position(pos), rotation(rot), scale(scl) { }

	virtual glm::mat4 getModelMatrix();

	glm::vec2 getPosition() const { return position; }
	void setPosition(const glm::vec2& pos) { position = pos; }
};

class MeshComponent : public Component {
private:
	bool visible;
	unsigned int meshId;
public:

	MeshComponent() = default;
	~MeshComponent() = default;
	MeshComponent(unsigned int id) 
	{ 
		meshId = id; 
		visible = true;
	}

	void turnVisible() {
		visible = true;
	}

	void turnInvisible() {
		visible = false;
	}

	unsigned int getId() {
		return meshId;
	}
};

class ColorComponent : public Component {
private:	
	glm::vec4 colorVec;
public:
	ColorComponent() = default;
	~ColorComponent() = default;
	void setColorVec(glm::vec4 cv) { this->colorVec = cv; }
	glm::vec4 getColorVec() { return colorVec; }
};

class Entity {
protected:
	static int nextId;
	int id;
	std::string name;
	bool active;

public:
	TransformComponent transform;
	MeshComponent mesh;
	ColorComponent color;

	Entity() = default;
	Entity(const std::string& entityName = "Entity");
	virtual ~Entity() = default;
	
	virtual void update(float deltaTime) = 0;
	virtual void onCollision(Entity* other);

	int getId() const { return id; }
	const std::string& getName() { return name;  }
	bool isActive() const { return active; }
	void turnActive() { active = true; }
	void turnInactive() { active = false; }
};
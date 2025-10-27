#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;

class Component {
private:
	bool active = true;
	Entity* owner = nullptr;

protected:
	// Metodi hook per le classi derivate
	virtual void onAttach() {}
	virtual void onDetach() {}

public:
	virtual ~Component() = default;

	// Ogni componente può sovrascrivere questi metodi
	virtual void update(float deltaTime) {}
	virtual void render() {} //Se necessario, perché magari vogliamo che alcuni componenti siano renderizzati

	// Stato del componente
	bool isActive() const { return active; }
	void setActive(bool flag) { active = flag; }

	// Gestione dell'ownership
	Entity* getOwner() const { return owner; }

	void attachOwner(Entity* entity) {
		if (owner != entity) {
			owner = entity;
			if (owner) {
				onAttach();
			}
		}
	}

	void detachOwner() {
		if (owner) {
			onDetach();
			owner = nullptr;
		}
	}

	bool isAttached() const { return owner != nullptr; }
};

#endif
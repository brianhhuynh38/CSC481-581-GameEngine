#include "GameObject.h"
#include "definitions.h"
#include "vector2D.h"
#include "json.hpp"
using json = nlohmann::json;

#include <iostream>
#include <map>
#include <vector>
#include <typeindex>

/*
* Some references include: https://www.gamedeveloper.com/design/the-entity-component-system---an-awesome-game-design-pattern-in-c-part-1-
* http://entity-systems.wikidot.com/test-for-parallel-processing-of-components#cpp
* Textbook
*/

// Default constructor for GameObject. Adds a Transform component (required)
GameObject::GameObject(){
	// Add default GameObject values
	m_uuid = -1;
	addComponent<Components::Transform>(
		Utils::Vector2D(0, 0), 
		nullptr, 
		Utils::Vector2D(1, 1), 
		Utils::Vector2D(1, 1)
	);
}

// Constructor with fields for GameObject. Adds a Transform component (required)
GameObject::GameObject(float scaleX, float scaleY, float positionX, float positionY, float width, float height, Utils::Vector2D *cameraPos) {
	// Add default GameObject ID
	m_uuid = -1;
	// Add Transform component, which is required for all GameObjects
	addComponent<Components::Transform>(
		Utils::Vector2D(positionX, positionY),
		cameraPos,
		Utils::Vector2D(width, height),
		Utils::Vector2D(scaleX, scaleY)
	);
}

// Update all components attached to game object and the time step the object should be using for calculations
void GameObject::update(double deltaTimeInSecs) {
	m_currTimeStep = deltaTimeInSecs;
	for (auto& [type, component] : m_components) {
		component->update();  // Update each component
	}
}

void GameObject::setUUID(int uuid) {
	this->m_uuid = uuid;
}

int GameObject::getUUID() {
	return m_uuid;
}

double GameObject::getDeltaTimeInSecsOfObject() {
	return m_currTimeStep;
}

void GameObject::from_json(const json& j) {
	if (j.contains("uuid")) {
		m_uuid = j["uuid"].get<int>();
	}
	// If it contains each component
	if (j.contains("transform")) {
		auto transformData = j["transform"];
		addComponent<Components::Transform>(
			Utils::Vector2D(transformData["position"]["x"], transformData["position"]["y"]),
			nullptr,  // cameraPos is handled elsewhere
			Utils::Vector2D(transformData["width"], transformData["height"]),
			Utils::Vector2D(transformData["scale"]["x"], transformData["scale"]["y"])
		);
	}
	// Other components here
	// TODO: Add all other components here
	if (j.contains("rigidbody")) {
		auto transformData = j["transform"];
		auto rigidBodyData = j["rigidbody"];
		addComponent<Components::RigidBody>(
			rigidBodyData["mass"],
			rigidBodyData["iskinematic"],
			SDL_Rect() = {
				(int)transformData["position"]["x"],
				(int)transformData["position"]["y"],
				(int)((float)transformData["scale"]["x"] * (float)transformData["width"]),
				(int)((float)transformData["scale"]["y"] * (float)transformData["height"])
			},
			rigidBodyData["collidertype"], // Default collider type
			this
		);
	}

	if (j.contains("texturemesh")) {
		auto textureMeshData = j["texturemesh"];
		addComponent<Components::TextureMesh>(
			
		);
	}

	if (j.contains("playerinput")) {
		auto playerInputData = j["playerinput"];
		addComponent<Components::PlayerInputPlatformer>(

		);
	}
}

void GameObject::to_json(json& j) {
	j["uuid"] = m_uuid;

	// If it contains each component
	Components::Transform* transform = getComponent<Components::Transform>();
	if (transform) {
		j["transform"] = {
			{"position", {{"x", transform->getPosition()->x}, {"y", transform->getPosition()->y}}},
			{"width", transform->getSize().x},
			{"height", transform->getSize().y},
			{"scale", {{"x", transform->getScale().x}, {"y", transform->getScale().y}}}
		};
	}
	// Other components here
	// TODO: Add all other components here
}

// Example for main:
// Player player(1, 1, 0, 0, 50, 50, 10, "player_texture.png", false, true, 0, 10, 5.0f);
// StaticPlatform platform(1, 1, 100, 50, 200, 10, 100, "platform_texture.png", true, false);

// Others to add:
// - MovingPlatform: Server-side.  TextureMesh, RigidBody, MovementController?
// - SpawnPoint: DON'T need to add, can just be a GameObject, only needs Transform
// - DeathZone: RigidBody (for collisions)
// - SideBoundary: RigidBody (for collisions)

#include "GameObject.h"
#include "definitions.h"
#include "vector2D.h"
//#include "transform.h"
//#include "rigidBody.h"

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
		Utils::Vector2D(0,0), 
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

// Example for main:
// Player player(1, 1, 0, 0, 50, 50, 10, "player_texture.png", false, true, 0, 10, 5.0f);
// StaticPlatform platform(1, 1, 100, 50, 200, 10, 100, "platform_texture.png", true, false);

// Others to add:
// - MovingPlatform: Server-side.  TextureMesh, RigidBody, MovementController?
// - SpawnPoint: DON'T need to add, can just be a GameObject, only needs Transform
// - DeathZone: RigidBody (for collisions)
// - SideBoundary: RigidBody (for collisions)

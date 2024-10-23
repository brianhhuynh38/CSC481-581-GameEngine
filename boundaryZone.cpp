#include "boundaryZone.h"
#include "GameObject.h"

#include <string>

BoundaryZone::BoundaryZone(float scaleX, float scaleY, float positionX, float positionY, float width, float height, Utils::Vector2D* cameraPos, float mass,
	std::string textureFilepath, bool isKinematic, Utils::Vector2D pos1, Utils::Vector2D pos2) {

	m_uuid = 0;

	/** The reference to the global camera offset value used to keep track of the camera's position */
	m_cameraOffset = cameraPos;
	m_pos1 = pos1;
	m_pos2 = pos2;
	m_timer = 0;

	// Adding specific components for StaticPlatform
	addComponent<Components::Transform>(
		Utils::Vector2D(positionX, positionY),
		Utils::Vector2D(width, height),
		Utils::Vector2D(scaleX, scaleY)
	);

	addComponent<Components::RigidBody>(
		mass,
		isKinematic,
		SDL_Rect() = { (int)positionX, (int)positionY, (int)(scaleX * width), (int)(scaleY * height) },
		2, // Boundary zone collider type
		this
	);
	addComponent<Components::TextureMesh>(textureFilepath);
}

void BoundaryZone::update(double deltaTimeInSecs) {
	//std::cout << "Update all game object components\n";
	m_currTimeStep = deltaTimeInSecs;
	for (auto& [type, component] : m_components) {
		component->update();  // Update each component
	}
	m_timer--;
}

Utils::Vector2D BoundaryZone::getCurrentPos() {
	return *m_cameraOffset;
}

void BoundaryZone::setCurrentPos(Utils::Vector2D newPos) {
	*m_cameraOffset = newPos;
}


Utils::Vector2D BoundaryZone::getPos1() {
	return m_pos1;
}

Utils::Vector2D BoundaryZone::getPos2() {
	return m_pos2;
}

void BoundaryZone::initiateTimer(int timeToSet) {
	m_timer = timeToSet;
}

bool BoundaryZone::checkCooldown() {
	return m_timer <= 0;
}

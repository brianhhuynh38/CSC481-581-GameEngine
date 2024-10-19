#include "playerGO.h"

#include <string>

PlayerGO::PlayerGO(float scaleX, float scaleY, float positionX, float positionY, float width, float height, float mass,
	std::string textureFilepath, bool isKinematic, float jumpVectorX, float jumpVectorY, float maxSpeed, InputHandler* inputHandler, GameObject* spawnPoint) {
	// Adding specific components for Player
	m_spawnPoint = spawnPoint;

	// Add transform component
	addComponent<Components::Transform>(
		Utils::Vector2D(positionX, positionY),
		Utils::Vector2D(width, height),
		Utils::Vector2D(scaleX, scaleY)
	);

	// Add RigidBody component
	addComponent<Components::RigidBody>(
		mass,
		isKinematic,
		SDL_Rect() = { (int)positionX, (int)positionY, (int)(scaleX * width), (int)(scaleY * height) },
		0, // Default collider type
		this
	);

	// Add TextureMesh component
	addComponent<Components::TextureMesh>(textureFilepath);

	// Add PlayerInput Component (specifically the platforming controls currently)
	addComponent<Components::PlayerInputPlatformer>(
		maxSpeed,
		Utils::Vector2D(jumpVectorX, jumpVectorY),
		inputHandler,
		this
	);
}

void PlayerGO::setSpawn(GameObject* spawnPoint) {
	m_spawnPoint = spawnPoint;
}

GameObject* PlayerGO::getSpawn() {
	return m_spawnPoint;
}
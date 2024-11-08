#include "playerGO.h"

#include "global.h"
#include "spawnEvent.h"
#include "deathEvent.h"

#include <string>

PlayerGO::PlayerGO() {
	m_spawnPoint = nullptr;

	// Add transform component
	addComponent<Components::Transform>(
		Utils::Vector2D(0.0f, 0.0f),
		Utils::Vector2D(1.0f, 1.0f),
		Utils::Vector2D(1.0f, 1.0f)
	);

	// Add RigidBody component
	addComponent<Components::RigidBody>(
		5.0f,
		false,
		SDL_Rect() = { (int)0, (int)0, (int)(1 * 1), (int)(1 * 1) },
		0, // Default collider type
		this
	);

	// Add TextureMesh component
	addComponent<Components::TextureMesh>("./Assets/Textures/DefaultPlayerTexture1.png");

	// Add PlayerInput Component (specifically the platforming controls currently)
	addComponent<Components::PlayerInputPlatformer>(
		0,
		Utils::Vector2D(0, 0),
		this
	);
	// Register the player GameObject for spawn and death events
	eventManager->registerEvent<Events::SpawnEvent>(this);
	eventManager->registerEvent<Events::DeathEvent>(this);
}

PlayerGO::PlayerGO(float scaleX, float scaleY, float positionX, float positionY, float width, float height, float mass,
	std::string textureFilepath, bool isKinematic, float jumpVectorX, float jumpVectorY, float maxSpeed, GameObject* spawnPoint) {
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
		this
	);
}

void PlayerGO::setSpawn(GameObject* spawnPoint) {
	m_spawnPoint = spawnPoint;
}

GameObject* PlayerGO::getSpawn() {
	return m_spawnPoint;
}
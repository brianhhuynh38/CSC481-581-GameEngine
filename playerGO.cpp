#include "playerGO.h"
#include "GameObject.h"

#include <string>

PlayerGO::PlayerGO(float scaleX, float scaleY, float positionX, float positionY, float width, float height, Utils::Vector2D* cameraPos, float mass,
	std::string textureFilepath, bool isKinematic, float jumpVectorX, float jumpVectorY,
	float maxSpeed) {
	// Adding specific components for Player
	addComponent<Components::Transform>(
		Utils::Vector2D(positionX, positionY),
		cameraPos,
		Utils::Vector2D(width, height),
		Utils::Vector2D(scaleX, scaleY)
	);

	addComponent<Components::RigidBody>(
		mass,
		isKinematic,
		SDL_Rect() = { (int)positionX, (int)positionY, (int)(scaleX * width), (int)(scaleY * height) },
		0, // Default collider type
		this
	);
	addComponent<Components::TextureMesh>(textureFilepath);
	addComponent<Components::PlayerInputPlatformer>();
}

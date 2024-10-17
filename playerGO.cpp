#include "playerGO.h"
#include "GameObject.h"

#include <string>

PlayerGO::PlayerGO(float scaleX, float scaleY, float positionX, float positionY, float width, float height, Utils::Vector2D* cameraPos, float mass,
	std::string textureFilepath, bool isStatic, bool isTrigger, float jumpVectorX, float jumpVectorY,
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
		isStatic,
		SDL_Rect() = { (int)positionX, (int)positionY, (int)(scaleX * width), (int)(scaleY * height) },
		isTrigger,
		this
	);
	addComponent<Components::TextureMesh>(textureFilepath);
	addComponent<Components::PlayerInputPlatformer>();
}

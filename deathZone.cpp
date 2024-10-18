#include "deathZone.h"
#include "GameObject.h"

#include <string>

DeathZone::DeathZone(float scaleX, float scaleY, float positionX, float positionY, float width, float height, Utils::Vector2D* cameraPos, float mass,
	std::string textureFilepath, bool isKinematic) {
	// Adding specific components for StaticPlatform
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
		1, // Death zone collider type
		this
	);
	addComponent<Components::TextureMesh>(textureFilepath);
}
#include "staticObject.h"


StaticObject::StaticObject(float scaleX, float scaleY, float positionX, float positionY, float width, float height, float mass,
	std::string textureFilepath, bool isKinematic) {
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
		0, // Default collider type (normal collision)
		this
	);
	addComponent<Components::TextureMesh>(textureFilepath);
}

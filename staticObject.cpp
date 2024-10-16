#include "staticObject.h"
#include "GameObject.h"

#include <string>

StaticObject::StaticObject(float scaleX, float scaleY, float positionX, float positionY, float width, float height, Utils::Vector2D* cameraPos, float mass,
	std::string textureFilepath, bool isStatic, bool isTrigger, Physics* physicsRef) {
	// Adding specific components for StaticPlatform
	addComponent<Components::Transform>(
		Utils::Vector2D(positionX, positionY),
		cameraPos,
		Utils::Vector2D(width, height),
		Utils::Vector2D(scaleX, scaleY)
	);

	//(float mass, bool isStatic, SDL_Rect collider, bool isTrigger, GameObject * parentRef, Physics * physicsRef)

	addComponent<Components::RigidBody>(
		mass,
		isStatic,
		SDL_Rect() = { (int)positionX, (int)positionY, (int)(scaleX * width), (int)(scaleY * height) },
		isTrigger,
		this,
		physicsRef
	);
	addComponent<Components::TextureMesh>(textureFilepath);
}
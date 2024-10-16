//#include "staticObject.h"
//
//#include "component.h"
//#include "transform.h"
//#include "rigidBody.h"
//#include "textureMesh.h"
//
//#include <string>
//
//StaticObject::StaticObject(float scaleX, float scaleY, float positionX, float positionY, float width, float height, float mass,
//	std::string textureFilepath, bool isStatic, bool isTrigger, Physics* physicsRef) {
//	// Adding specific components for StaticPlatform
//	addComponent<Components::Transform>(scaleX, scaleY, positionX, positionY, width, height);
//	addComponent<Components::RigidBody>(mass, isStatic, SDL_Rect() = { (int)positionX, (int)positionY, (int)(scaleX * width), (int)(scaleY * height) }, isTrigger, this, physicsRef);
//	addComponent<Components::TextureMesh>(textureFilepath);
//}
//

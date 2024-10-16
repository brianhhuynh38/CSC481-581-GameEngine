//#include "playerGO.h"
//
//#include "transform.h"
//#include "rigidBody.h"
//#include "textureMesh.h"
//#include "playerInput.h"
//
//#include <string>
//#include <SDL.h>
//
//PlayerGO::PlayerGO(float scaleX, float scaleY, float positionX, float positionY, float width, float height, float mass,
//	std::string textureFilepath, bool isStatic, bool isTrigger, float jumpVectorX, float jumpVectorY,
//	float maxSpeed, Physics* physicsRef) {
//	// Adding specific components for Player
//	addComponent<Components::Transform>(scaleX, scaleY, positionX, positionY, width, height);
//	addComponent<Components::RigidBody>(mass, isStatic, SDL_Rect() = { (int)positionX, (int)positionY, (int)(scaleX * width), (int)(scaleY * height) }, isTrigger, this);
//	addComponent<Components::TextureMesh>(textureFilepath);
//	//addComponent<Components::PlayerInputPlatformer>();
//}

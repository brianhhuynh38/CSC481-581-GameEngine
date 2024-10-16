#pragma once

#ifndef STATICOBJECT_H
#define STATICOBJECT_H

#include "GameObject.h"
#include "physics.h"
#include "rigidBody.h"
#include "textureMesh.h"

// - StaticObject: TextureMesh, RigidBody (collision)
class StaticObject : public GameObject {
public:
	StaticObject(float scaleX, float scaleY, float positionX, float positionY, float width, float height, Utils::Vector2D* cameraPos, float mass,
		std::string textureFilepath, bool isStatic, bool isTrigger, Physics* physicsRef);
};

#endif

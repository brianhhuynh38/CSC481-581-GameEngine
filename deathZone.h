#pragma once

#ifndef DEATHZONE_H
#define DEATHZONE_H

#include "GameObject.h"
#include "rigidBody.h"
#include "textureMesh.h"

class DeathZone : public GameObject {
public:
	DeathZone(float scaleX, float scaleY, float positionX, float positionY, float width, float height, float mass,
		std::string textureFilepath, bool isKinematic);
};

#endif

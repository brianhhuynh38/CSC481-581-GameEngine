#pragma once

#ifndef BOUNDARYZONE_H
#define BOUNDARYZONE_H

#include "GameObject.h"
#include "rigidBody.h"
#include "textureMesh.h"

class BoundaryZone : public GameObject {
public:
	BoundaryZone(float scaleX, float scaleY, float positionX, float positionY, float width, float height, Utils::Vector2D* cameraPos, float mass,
		std::string textureFilepath, bool isKinematic);
};

#endif

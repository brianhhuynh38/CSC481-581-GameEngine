#pragma once

#ifndef PLAYERGO_H
#define PLAYERGO_H

#include "GameObject.h"
#include "structs.h"
#include "rigidBody.h"
#include "textureMesh.h"
#include "playerInput.h"

// - Player: TextureMesh, RigidBody (collision), PlayerInput
class PlayerGO : public GameObject {
public:
    PlayerGO(float scaleX, float scaleY, float positionX, float positionY, float width, float height, Utils::Vector2D* cameraPos, float mass,
		std::string textureFilepath, bool isKinematic, float jumpVectorX, float jumpVectorY, float maxSpeed, InputHandler* inputHandler);
};

#endif

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
protected:
	/* Reference to the Player's spawn point. */
	GameObject* m_spawnPoint;
public:
	/** 
	 * Default constructor
	 */
	PlayerGO();
	
	/**
	 * Constructor
	 */
    PlayerGO(float scaleX, float scaleY, float positionX, float positionY, float width, float height, float mass,
		std::string textureFilepath, bool isKinematic, float jumpVectorX, float jumpVectorY, float maxSpeed, GameObject* spawnPoint);

	void setSpawn(GameObject* spawnPoint);

	GameObject* getSpawn();
};

#endif

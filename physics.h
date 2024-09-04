#include "vector2D.h"
#include "entity.h"

#pragma once

/**
* The class that handles physics interactions
*/
class Physics {
private:
	/** The value of gravity */
	float m_gravity;
public:
	/**
	* The physics constructor that also sets the gravity value
	*/
	Physics(float gravity);

	/**
	* Updates all physics vectors for each entity through simple physics calculations and timeline usage
	*/
	void updateEntityPhysicsVectors(Entities::Entity entity);

	/**
	* Applies gravity to all entities' acceleration vectors
	*/
	void applyGravity(Entities::Entity entity);
};

#include "vector2D.h"
#include "entity.h"
#include "timeline.h"

#pragma once

/**
* The class that handles physics interactions
*/
class Physics {
private:
	/** The value of gravity */
	float m_gravity;
public:
  
	Physics();

	void setGravity(float gravity);
	
    float getGravity();

	/**
	* Updates all physics vectors for each entity through simple physics calculations and timeline usage
	*/
	void updateEntityPhysicsVectors(Timeline *timeline, Entities::Entity *entity);

	void updatePhysicsVectors(double deltaTimeInSecs, Utils::Vector2D* position, Utils::Vector2D* velocity, Utils::Vector2D* acceleration);

	/**
	 * Applies the given force to the given entity
	 * @param entity to apply force too
	 * @param forceVector to appluy
	 */
	void applyForce(Entities::Entity *entity, Utils::Vector2D forceVector);

	/**
	* Applies gravity to all entities' acceleration vectors
	*/
	void applyGravity(Entities::Entity *entity, Timeline *timeline);

	void applyGravity(double deltaTimeInSecs, float mass, Utils::Vector2D* acceleration);

};

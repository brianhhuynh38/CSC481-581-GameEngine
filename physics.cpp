#include "physics.h"
#include "global.h"

Physics::Physics() {
	// default
	m_gravity = 9.81;
}

/**
* Updates the entity's position and velocity by calculating each value using 
* the values of their derivative counterparts. They are calculated in this 
* order so that they update with values of THIS time step and not the next.
* 
* @param entity: The entity being updated
*/
void Physics::updateEntityPhysicsVectors(Entities::Entity entity) {
	// Get delta time to find and update velocity and acceleration
	int deltaTime = timeline.getDeltaTime();
	// Update position using velocity
	entity.updatePosition(entity.getVelocity()->multConst(deltaTime));
	// Update velocity using acceleration
	entity.updateVelocity(entity.getAcceleration()->multConst(deltaTime));
}

/**
* Updates the entity's current acceleration to have gravity's influence
* 
* @param entity: The entity to whom gravity is being applied
*/
void Physics::applyGravity(Entities::Entity entity) {
	// Gravity value is multiplied by 01 to make it a downward force
	entity.updateAcceleration(Utils::Vector2D(0, m_gravity * -1));
}

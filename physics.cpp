#include "physics.h"
#include "global.h"

Physics::Physics() {
	// default value
	m_gravity = 9.81f;
}

/**
* Sets the gravity value
*/
void Physics::setGravity(float gravity) {
	m_gravity = gravity;
}

/**
* Updates the entity's position and velocity by calculating each value using 
* the values of their derivative counterparts. They are calculated in this 
* order so that they update with values of THIS time step and not the next.
* 
* @param entity: The entity being updated
*/
void Physics::updateEntityPhysicsVectors(Entities::Entity *entity) {
	// Get delta time to find and update velocity and acceleration
	int deltaTime = timeline.getDeltaTime();

	applyGravity(entity);

	// Update velocity using acceleration
	entity->updateVelocity(entity->getAcceleration()->multConst(deltaTime));
	// Update position using velocity
	entity->updatePosition(entity->getVelocity()->multConst(deltaTime));
	std::cout << "DT:" << deltaTime << "\n";
}

/**
 * Applies the given force to the given entity
 * @param entity to apply force too
 * @param forceVector to apply
 */
void Physics::applyForce(Entities::Entity *entity, Utils::Vector2D forceVector) {
	entity->updateAcceleration(forceVector.divideConst(entity->getMass()));
	std::cout << "applyForce: (" << forceVector.x << ", " << forceVector.y << ")\n";
}

/**
* Updates the entity's current acceleration to have gravity's influence
* 
* @param entity: The entity to whom gravity is being applied
*/
void Physics::applyGravity(Entities::Entity *entity) {
	// Gravity value is multiplied by 01 to make it a downward force
	entity->updateAcceleration(Utils::Vector2D(0, m_gravity));
}

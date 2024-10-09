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
	std::cout << "Gravity:" << gravity << "\n";
	m_gravity = gravity;
}

/**
* Gets the gravity value
*/
float Physics::getGravity() {
	return m_gravity;
}

/**
* Updates the entity's position and velocity by calculating each value using 
* the values of their derivative counterparts. They are calculated in this 
* order so that they update with values of THIS time step and not the next.
* 
* @param entity: The entity being updated
*/
void Physics::updateEntityPhysicsVectors(Timeline *timeline, Entities::Entity *entity) {
	// Get delta time to find and update velocity and acceleration
	double deltaTimeInSecs = timeline->getDeltaTime() / MICROSEC_PER_SEC;
	//std::cout << "Delta Time from Function: " << timeline.getDeltaTime() << "\n";
	//std::cout << "Delta Time in Seconds: " << deltaTimeInSecs << "\n";

	applyGravity(entity, timeline);

	// Update velocity using acceleration
	//entity->updateVelocity(entity->getAcceleration()->add(0, m_gravity * entity->getMass()).multConst(deltaTimeInSecs));
	entity->updateVelocity(entity->getAcceleration()->multConst(deltaTimeInSecs));

	/*std::cout << "Velocity: " << entity->getVelocity()->y;*/
	// Update position using velocity
	entity->updatePosition(entity->getVelocity()->multConst(deltaTimeInSecs));
}

/**
 * Applies the given force to the given entity
 * @param entity to apply force too
 * @param forceVector to apply
 */
void Physics::applyForce(Entities::Entity *entity, Utils::Vector2D forceVector) {
	entity->updateAcceleration(forceVector.divideConst(entity->getMass()));
	/*std::cout << "applyForce: (" << forceVector.x << ", " << forceVector.y << ")\n";*/
}

/**
* Updates the entity's current acceleration to have gravity's influence
* 
* @param entity: The entity to whom gravity is being applied
*/
void Physics::applyGravity(Entities::Entity *entity, Timeline *timeline) {
	double deltaTimeInSecs = timeline->getDeltaTime() / MICROSEC_PER_SEC;

	Utils::Vector2D gravityForce(0, m_gravity * entity->getMass());

	// Gravity value is multiplied by 01 to make it a downward force
	//entity->updateVelocity(entity->getVelocity()->add(gravityForce.multConst(deltaTimeInSecs)));
	entity->updateAcceleration(Utils::Vector2D(0, m_gravity * entity->getMass()).multConst(deltaTimeInSecs));
	//entity->updateAcceleration(Utils::Vector2D(0, m_gravity * timeline.getDeltaTime() / MICROSEC_PER_SEC));
	//entity->setAcceleration(entity->getAcceleration()->x, (m_gravity + entity->getAcceleration()->y) * (1.0f - timeline->getDeltaTime() / MICROSEC_PER_SEC));
}

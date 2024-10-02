#include "physics.h"
#include "global.h"
#include "collisions.h"

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
	double deltaTimeInSecs = timeline.getDeltaTime() / MICROSEC_PER_SEC;
	//std::cout << "Delta Time from Function: " << timeline.getDeltaTime() << "\n";
	//std::cout << "Delta Time in Seconds: " << deltaTimeInSecs << "\n";

	applyGravity(entity);

	// Update velocity using acceleration
	entity->updateVelocity(entity->getAcceleration()->multConst(deltaTimeInSecs));

	// Update position using velocity
	
	Utils::Vector2D movementVector = entity->getVelocity()->multConst(deltaTimeInSecs);
	// moves and checks for collision on each axis individually while updating the entities position
	if (movementVector.x != 0) {
		entity->updatePosition(Utils::Vector2D(movementVector.x, 0));
		collisionCheck(entity, Utils::Vector2D(movementVector.x, 0));
		//std::cout << "mx\n";
	}
	if (movementVector.y != 0) {
		entity->updatePosition(Utils::Vector2D(0, movementVector.y));
		collisionCheck(entity, Utils::Vector2D(0, movementVector.y));
		//std::cout << "my\n";
	}
	std::cout << "G1: " << entity->getIsGrounded() << "\n";
}

/**
 * Applies the given force to the given entity
 * @param entity to apply force too
 * @param forceVector to apply
 */
void Physics::applyForce(Entities::Entity *entity, Utils::Vector2D forceVector) {
	entity->updateAcceleration(forceVector.divideConst(entity->getMass()));
	//std::cout << "applyForce: (" << forceVector.x << ", " << forceVector.y << ")\n";
}

/**
* Updates the entity's current acceleration to have gravity's influence
* 
* @param entity: The entity to whom gravity is being applied
*/
void Physics::applyGravity(Entities::Entity *entity) {
	//applyForce(entity, Utils::Vector2D(0, m_gravity * entity->getMass()).multConst(timeline.getDeltaTime() / MICROSEC_PER_SEC));
	if (entity->getAcceleration()->y < m_gravity) {
		entity->updateAcceleration(Utils::Vector2D(0, m_gravity * entity->getMass()).multConst(timeline.getDeltaTime() / MICROSEC_PER_SEC));
	}
	else {
		entity->setAcceleration(entity->getAcceleration()->x, m_gravity);
	}
	//entity->updateAcceleration(Utils::Vector2D(0, m_gravity * entity->getMass()).multConst(timeline.getDeltaTime() / MICROSEC_PER_SEC));
	
	
}
/**
 * Helper function for checking for any collisions that may have occured
 */
void Physics::collisionCheck(Entities::Entity *entity, Utils::Vector2D movementVector) {


	// Create colliders iterator
	std::list<SDL_Rect>::iterator iterCol;
	std::list<SDL_Rect> *m_colliders = entity->getColliders();

	//std::cout << "Player coordinates: " << m_position->x << ", " << m_position->y << "\n";
	// Loop through colliders
	for (iterCol = m_colliders->begin(); iterCol != m_colliders->end(); ++iterCol) {
		// Move the colliders the calculated distance
		iterCol->x = entity->getPosition()->x;
		iterCol->y = entity->getPosition()->y;
		//std::cout << "Collider coordinates: " << iterCol->x << "," << iterCol->y << "\n";
	}

	// Checks for collisions
	HitInfo hInfo = checkCollisions(m_colliders, entityController->getEntities());

	// If the object collided with something
	if (hInfo.hit) {
		if (movementVector.x != 0) {
			std::cout << "X-HIT!\n";
			
		}
		else if (movementVector.y != 0) {
			std::cout << "Y-HIT!\n";
			
		}

		entity->updatePosition(movementVector.multConst(-1));
		entity->updateAcceleration(movementVector.multConst(-1));
		entity->updateVelocity(movementVector.multConst(-1));


		// Create colliders iterator
		std::list<SDL_Rect>::iterator iterCol2;

		for (iterCol2 = m_colliders->begin(); iterCol2 != m_colliders->end(); ++iterCol2) {
			// Move the colliders the calculated distance
			//iterCol2->x = oldPosition.x;
			//iterCol2->y = oldPosition.y;
			iterCol2->x = entity->getPosition()->x;
			iterCol2->y = entity->getPosition()->y;

			//std::cout << "COLLIDED coordinates: " << iterCol2->x << "," << iterCol2->y << "\n";
		}

		// checks if hit occured at bottom of collider. If so, sets entity to "grounded"
		if (movementVector.y != 0) {
			entity->setIsGrounded((entity->getVelocity()->y > 0));
		}
		

	}
}
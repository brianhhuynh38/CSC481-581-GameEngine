#include "entityController.h"
#include "global.h"
#include "draw.h"

EntityController::EntityController() {
	// creates empty entities list
	m_entities = new std::list<Entities::Entity>();
	m_movingEntities = new std::list<Entities::MovingEntity>();
}

void EntityController::updateEntities() {
	// Create list iterator
	std::list<Entities::Entity>::iterator iter;
	// Updates the physics vectors for each entity in the list of entities that is tagged as "affectedByPhysics"
	for (iter = (*m_entities).begin(); iter != (*m_entities).end(); ++iter) {
		if ((*iter).getAffectedByPhysics()) {
			physics.updateEntityPhysicsVectors(&(*iter));
			//std::cout << "phys";
		}
	}

	std::list<Entities::MovingEntity>::iterator movIter;
	// Updates the physics vectors for each entity in the list of entities that is tagged as "affectedByPhysics"
	for (movIter = (*m_movingEntities).begin(); movIter != (*m_movingEntities).end(); ++movIter) {
		movIter->move();
	}
}

std::list<Entities::Entity> *EntityController::getEntities() {
	return m_entities;
}

/**
 * Adds the given entity to the entities list
 * @param e entity to be added to end of the entities list
 */
void EntityController::addEntity(Entities::Entity e) {
	m_entities->emplace_back(e);
}

/**
 * Adds the given entity to the entities list
 * @param e entity to be added to end of the entities list
 */
void EntityController::addMovingEntity(Entities::MovingEntity e) {
	m_movingEntities->emplace_back(e);
}

void EntityController::cleanUp() {
	// TODO: Free allocated memory
	// TODO: free memory on entities with the destroy function
	delete m_entities;
}

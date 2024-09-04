#include "entityController.h"
#include "global.h"

EntityController::EntityController() {
	// creates empty entities list
	m_entities = {};
	// TODO: Figure out way to store and load all entities to be used in the game
}

void EntityController::updateEntities() {
	// Create list iterator
	std::list<Entities::Entity>::iterator iter;
	// Updates the physics vectors for each entity in the list of entities
	for (iter = (*m_entities).begin(); iter != (*m_entities).end(); ++iter) {
		physics.updateEntityPhysicsVectors(*iter);
	}
}

std::list<Entities::Entity> EntityController::getEntities() {
	return *m_entities;
}

/**
 * Adds the given entity to the entities list
 * @param e entity to be added to end of the entities list
 */
void EntityController::addEntity(Entities::Entity e) {
	m_entities->emplace_back((Entities::Entity)e);
}

void EntityController::cleanUp() {
	// TODO: Free allocated memory
}

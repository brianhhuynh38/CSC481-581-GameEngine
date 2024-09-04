#include "entityController.h"
#include "global.h"

EntityController::EntityController() {
	// creates empty entities list
	m_entities = new std::list<Entities::Entity>();
	m_entities->push_back(Entities::Entity());
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
	m_entities->emplace_back(e);
}

void EntityController::cleanUp() {
	// TODO: Free allocated memory
	delete m_entities;
}

#include "entity.h"
#include <list>

#pragma once

/**
* Manages and updates physics and functions of all entities
*/
class EntityController {
private:
	/** The list of all entities */
	std::list<Entities::Entity> *m_entities;
public:
	/**
	* The constructor for EntityController
	*/
	EntityController();

	/**
	* Updates each entity's logic and physics
	*/
	void updateEntities();

	/**
	* Returns the entities list
	*/
	std::list<Entities::Entity> getEntities();

	/**
	* Frees any allocated memory
	*/
	void cleanUp();
};

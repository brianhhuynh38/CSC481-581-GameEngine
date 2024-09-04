#include "entity.h"
#include <list>

#pragma once

/**
* Manages and updates physics and functions of all entities
*/
class EntityController {
private:
	std::list<Entities::Entity> m_entities;
public:
	EntityController();

	void updateEntities();

	void cleanUp();
};

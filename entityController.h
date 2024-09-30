#include "entity.h"
#include "movingEntity.h"
#include <list>
#include <SDL.h>
#include <iostream>

#pragma once

/**
* Manages and updates physics and functions of all entities
*/
class EntityController {
private:
	/** The list of all entities */
	std::list<Entities::Entity> *m_entities;
	/** The list of all entities */
	std::list<Entities::MovingEntity> *m_movingEntities;
	/** The map of all player entities that are not controlled by this client */
	std::map<int, Entities::Entity>* m_opposingPlayers;
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
	* Deserializes a string and adds those entities into the entity controller
	* 
	* @param entityListString
	*/
	void updateEntitiesByString(std::string entityListString);

	/**
	* Returns the entities list
	*/
	std::list<Entities::Entity> *getEntities();

	/**
	* Adds the given entity to the entities list
	* @param e entity to be added to end of the entities list
	*/
	void addEntity(Entities::Entity e);

	/**
	* Adds the given entity to the entities list
	* @param e entity to be added to end of the entities list
	*/
	void addMovingEntity(Entities::MovingEntity e);

	/**
	* Frees any allocated memory
	*/
	void cleanUp();
};

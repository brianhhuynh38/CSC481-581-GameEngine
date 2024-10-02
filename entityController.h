#include "entity.h"
#include "movingEntity.h"
#include "player.h"
#include "physics.h"
#include "timeline.h"
#include <list>
#include <SDL.h>
#include <iostream>
#include <map>

#pragma once

/**
* Manages and updates physics and functions of all entities
*/
class EntityController {
private:
	/** The list of all entities */
	std::map<int, Entities::Entity> *m_entities;
	/** The list of all entities */
	std::map<int, Entities::MovingEntity> *m_movingEntities;
	/** The map of all player entities that are not controlled by this client */
	std::map<int, Entities::Entity>* m_opposingPlayers;
	/** The ID the client is using */
	int m_playerID;
	/** Physics handler pointer */
	Physics *physics;
public:
	/**
	* The constructor for EntityController
	*/
	EntityController(Physics *physics);

	/**
	* Updates each entity's logic and physics
	*/
	void updateEntities(Timeline *timeline);

	/**
	* Deserializes a string and adds those entities into the entity controller
	* 
	* @param entityListString
	*/
	void updateEntitiesByString(std::string entityListString);

	/**
	* Sets the player ID that is ignored when receiving information from the server
	*/
	void setPlayerID(int uuid);

	/**
	* Returns the entities list
	*/
	std::map<int, Entities::Entity> *getEntities();

	/**
	* Adds the given entity to the entities list
	* @param e entity to be added to end of the entities list
	*/
	void insertEntity(Entities::Entity e);

	/**
	* Adds the given entity to the entities list
	* @param e entity to be added to end of the entities list
	*/
	void insertMovingEntity(Entities::MovingEntity e);

	/**
	* Adds the given opposing player to the entities list
	* @param e entity to be added to end of the entities list
	*/
	void insertOpposingPlayer(Entities::Player p);

	/**
	* Frees any allocated memory
	*/
	void cleanUp();
};

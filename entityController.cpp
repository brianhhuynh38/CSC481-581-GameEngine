#include <map>
#include <iostream>
#include <sstream>

#include "entityController.h"
#include "player.h"
#include "global.h"
#include "draw.h"

EntityController::EntityController(Physics *physics) {
	// creates empty entities list
	m_entities = new std::map<int, Entities::Entity>();
	m_movingEntities = new std::map<int, Entities::MovingEntity>();
	m_opposingPlayers = new std::map<int, Entities::Entity>();
	m_playerID = -100;
	this->physics = physics;
}

void EntityController::updateEntities(Timeline *timeline) {
	// Create list iterator
	std::map<int, Entities::Entity>::iterator iter;
	// Updates the physics vectors for each entity in the list of entities that is tagged as "affectedByPhysics"
	for (iter = m_entities->begin(); iter != m_entities->end(); ++iter) {
		int entityId = iter->first; // Get the ID of the current entity

		// Check if the entity is affected by physics and is not an opposing player
		if (iter->second.getAffectedByPhysics() && m_opposingPlayers->find(entityId) == m_opposingPlayers->end()) {
			physics->updateEntityPhysicsVectors(timeline, &iter->second);
			//std::cout << "phys";
		}
	}

	//std::map<int, Entities::MovingEntity>::iterator movIter;
	//// Updates the physics vectors for each entity in the list of entities that is tagged as "affectedByPhysics"
	//for (movIter = m_movingEntities->begin(); movIter != m_movingEntities->end(); ++movIter) {
	//	movIter->second.move();
	//}
}

/**
	* Deserializes a string and adds those entities into the entity controller
	*
	* @param entityListString
	*/
void EntityController::updateEntitiesByString(std::string entityListString) {
	//std::cout << "EntityListString: " << entityListString << "\n";
	// Delimit by section
	std::stringstream ss(entityListString);
	// Strings for storing player and moving data
	std::string playersLine;
	std::string movingLine;
	// Temporary string to allocate information
	std::string temp;
	// Dispose of first identifier line
	std::getline(ss, temp);

	// Separate players and moving entities
	std::getline(ss, playersLine, '+');
	std::getline(ss, movingLine, '+');
	// Setup stringstreams for each part
	std::stringstream ssPlayer(playersLine);
	std::stringstream ssMoving(movingLine);

	//std::cout << "Print out the EntityController list sizes:\n" << "Players: " << m_opposingPlayers->size() << "\n" << "Movers: " << m_movingEntities->size() << "\n";

	// Get rid of throwaway lines
	std::getline(ssPlayer, temp, '*');
	std::getline(ssMoving, temp, '*');

	// Insert player data except for data for this client
	while (std::getline(ssPlayer, temp, '*')) {
		// Construct player from string
		Entities::Player p = *Entities::Player::fromString(temp);
		// If the player is not the one this client controls, do not update
		if (p.getUUID() != m_playerID) {
			insertOpposingPlayer(p);
			insertEntity(p);
		}
	}

	// Insert moving entity data
	while (std::getline(ssMoving, temp, '*')) {
		//std::cout << "MovingEntity list size: " << m_movingEntities->size() << "\n";
		Entities::MovingEntity m = *Entities::MovingEntity::fromString(temp);
		// Insert the moving entity into the list
		insertMovingEntity(m);
		insertEntity(m);
	}
}

std::map<int, Entities::Entity> *EntityController::getEntities() {
	return m_entities;
}


void EntityController::setPlayerID(int uuid) {
	m_playerID = uuid;
}

/**
 * Adds the given entity to the entities list
 * @param e entity to be added to end of the entities list
 */
void EntityController::insertEntity(Entities::Entity e) {
	m_entities->insert_or_assign(e.getUUID(), e);
}

/**
 * Adds the given moving entity to the moving entities list
 * @param m entity to be added to end of the entities list
 */
void EntityController::insertMovingEntity(Entities::MovingEntity m) {
	m_movingEntities->insert_or_assign(m.getUUID(), m);
}

/**
 * Adds the given player to the opposing players list
 * @param p entity to be added to end of the entities list
 */
void EntityController::insertOpposingPlayer(Entities::Player p) {
	m_opposingPlayers->insert_or_assign(p.getUUID(), p);
}

void EntityController::cleanUp() {
	// TODO: Free allocated memory
	// TODO: free memory on entities with the destroy function
	delete m_entities;
}

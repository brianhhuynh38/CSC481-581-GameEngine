#include <map>
#include <iostream>
#include <sstream>
#include <vector>

#include "entityController.h"
#include "player.h"
#include "global.h"
#include "draw.h"

EntityController::EntityController() {
	// creates empty entities list
	m_entities = new std::map<int, Entities::Entity>();
	m_movingEntities = new std::map<int, Entities::MovingEntity>();
	m_opposingPlayers = new std::map<int, Entities::Entity>();
	m_playerID = -100;
}

void EntityController::updateEntities() {
	// Create list iterator
	std::map<int, Entities::Entity>::iterator iter;
	// Updates the physics vectors for each entity in the list of entities that is tagged as "affectedByPhysics"
	for (iter = m_entities->begin(); iter != m_entities->end(); ++iter) {
		if (iter->second.getAffectedByPhysics()) {
			physics.updateEntityPhysicsVectors(&iter->second);
			//std::cout << "phys";
		}
	}

	std::map<int, Entities::MovingEntity>::iterator movIter;
	// Updates the physics vectors for each entity in the list of entities that is tagged as "affectedByPhysics"
	for (movIter = m_movingEntities->begin(); movIter != m_movingEntities->end(); ++movIter) {
		movIter->second.move();
	}
}

/**
	* Deserializes a string and adds those entities into the entity controller
	*
	* @param entityListString
	*/
void EntityController::updateEntitiesByString(std::string entityListString) {
	// Delimit by section
	std::stringstream ss(entityListString);
	// Strings for storing player and moving data
	std::string playersLine;
	std::string movingLine;
	// Temporary string to allocate information
	std::string temp;
	// Dispose of first identifier line
	std::getline(ss, temp);

	// Helper functions
	// auto getFloat = [&]() { std::getline(ss, line); return std::stof(line); };
	// auto getInt = [&]() { std::getline(ss, line); return std::stoi(line); };
	// auto getBool = [&]() { return getInt() != 0; };

	// Separate players and moving entities
	std::getline(ss, playersLine, '+');
	std::getline(ss, movingLine, '+');
	// Setup stringstreams for each part
	std::stringstream ssPlayer(playersLine);
	std::stringstream ssMoving(movingLine);
	// Vectors to store individual entity lines
	std::vector<std::string> playerEntityLines;
	std::vector<std::string> movingEntityLines;

	

	while (std::getline(ssPlayer, temp, '*')) {
		insertOpposingPlayer(*Entities::Player::fromString(temp));
	}

	while (std::getline(ssMoving, temp, '*')) {
		movingEntityLines.push_back(temp);
	}

	// use insert methods below to insert or assign updates (do NOT add/update your own player, only opposing players)
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

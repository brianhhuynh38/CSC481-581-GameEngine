#include "gameObjectManager.h"

#include "component.h"
#include "rigidBody.h"
#include "definitions.h"

/**
* Constructor for the GameObjectManager that takes in a reference to the Timeline
*
* @param timeline: Reference to the timeline
*/
GameObjectManager::GameObjectManager(Timeline* timelineRef) {
	// Set starting ID value
	m_idTracker = 0;
	// Set reference to timeline
	m_timeline = timelineRef;
	// Instantiate empty map of GameObjects with UUIDs as the key
	m_objects = new std::map<int, GameObject*>();
}

/**
* Destructor that frees any allocated memory for the GameObjects
*/
GameObjectManager::~GameObjectManager() {
	delete m_objects;
}

/**
* Updates each of the GameObjects in the objects map
*/
void GameObjectManager::update() {
	// Calculate current delta time in seconds
	double deltaTimeInSecs = m_timeline->getDeltaTime() / MICROSEC_PER_SEC;
	// Create iterator to iterate through the Map
	std::map<int, GameObject*>::iterator iter;
	// Updates each GameObject in the map
	for (iter = m_objects->begin(); iter != m_objects->end(); ++iter) {
		iter->second->update(deltaTimeInSecs);
	}
}

/**
* Deserializes a string of movingObjects and inserts those GameObjects into the object map.
* This is meant to read in movingObject information sent from the server
*
* @param movingEntityString: string containing movingObject information from the server
* @param networkType: defines the type of network being used (1=client2server, 2=peer2peer)
*/
void GameObjectManager::serializeIn(std::string movingObjectString, int networkType) {
	// TODO: Create new serialization function for MovingObjects once that's implemented into the Server
}

/**
* Returns a pointer to the objects map
*/
std::map<int, GameObject*>* GameObjectManager::getObjectMap() {
	return m_objects;
}

/**
* Inserts the GameObject into the objects map
*
* @param go GameObject to be added to end of the object map
*/
void GameObjectManager::insert(GameObject* go) {
	// Sets UUID for the inserted object before adding it, if new
	if (go->getUUID() < 0) {
		go->setUUID(m_idTracker);
		m_idTracker++;
	}
	
	// Adds or inserts existing information into the Manager
	m_objects->insert_or_assign(go->getUUID(), go);
}


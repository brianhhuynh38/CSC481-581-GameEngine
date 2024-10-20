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

	m_serverObjects = new std::map<int, GameObject>();
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
		// Get GameObject
		GameObject* go = iter->second;
		// Update each GameObject component
		go->update(deltaTimeInSecs);
		// If GameObject has a RigidBody, update collisions
		if (Components::RigidBody *rb = go->getComponent<Components::RigidBody>()) {
			rb->updateCollisions(*m_objects);
		}
	}
}

/**
* Deserializes a string of gameObjects and inserts those GameObjects into the object map.
* This is meant to read in gameObject information sent from the server
*
* @param movingEntityString: string containing movingObject information from the server
* @param networkType: defines the type of network being used (1=client2server, 2=peer2peer)
*/
void GameObjectManager::deserialize(std::string gameObjectString, int networkType) {
	// TODO: Create new serialization function for MovingObjects once that's implemented into the Server
	json j = json::parse(gameObjectString);

	// Loop through objects in JSON array
	for (const auto& obj : j) {
		GameObject* go = new GameObject();
		// Get game object information from json
		go->from_json(obj);
		// insert game object into objects table
		//insert(go);
		
	}

	// Handle network type if necessary
}

/**
* Serializes a string of gameObjects and puts them all into one string.
* This is meant to prepare the gameObject information to send (might only need player itself)
*
* @param gameObjectString: string containing movingObject information from the server
* @param networkType: defines the type of network being used (1=client2server, 2=peer2peer)
*/
void GameObjectManager::serialize(std::string& outputString) {
	// TODO: Adjust for use in peer-to-peer where only the player's information needs to be sent.
	// Probably don't need to iterate, but need to have the local player field for conversion
	json j;

	for (const auto& [id, go] : *m_objects) {
		json gameObjectJson;
		go->to_json(gameObjectJson);
		j.push_back(gameObjectJson);
	}

	outputString = j.dump(); // Convert JSON to string
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
	if (go->getUUID() == 0) {
		go->setUUID(m_idTracker);
		m_idTracker--;
	}

	// TODO: It may be better to change so that it updates each of the components individually
	// Might also want to make it so that it doesn't try to load texture in the constructor so that
	// it doesn't call it every single time this function is called like last time
	
	// Adds or inserts existing information into the Manager
	m_objects->insert_or_assign(go->getUUID(), go);
}
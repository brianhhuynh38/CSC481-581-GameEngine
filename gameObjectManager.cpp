#include "gameObjectManager.h"

#include "component.h"
#include "playerInput.h"
#include "rigidBody.h"

#include "eventManager.h"
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
	// Instantiate empty map of gameObject meant to store client object references
	m_clientObjects = new std::map<int, GameObject*>();
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
	int64_t currentTime = m_timeline->getTime();
	// Create iterator to iterate through the Map
	std::map<int, GameObject*>::iterator iter;
	// Updates each GameObject in the map
	for (iter = m_objects->begin(); iter != m_objects->end(); ++iter) {
		// Get GameObject and set current ID being updated for mutex
		GameObject* go = iter->second;
		{
			std::lock_guard<std::mutex> lock(go->mutex);
			// Update each GameObject component
			go->update(deltaTimeInSecs, currentTime);
			// If GameObject has a RigidBody, update collisions
			if (Components::RigidBody* rb = go->getComponent<Components::RigidBody>()) {
				if (go->getUUID() == m_playerID) {
					rb->updateCollisions(*m_objects);
				}
			}
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
std::set<int> GameObjectManager::deserialize(std::string gameObjectString, int networkType) {
	// Parse json
	json j = json::parse(gameObjectString);

	// Create vector of integers: the IDs of any new player clients added
	std::set<int> intSet = std::set<int>();

	// Loop through objects in JSON array
	for (const auto& obj : j) {
		auto uuid = obj["uuid"];
		
		if (!m_objects->count(uuid)) { // If it's a new game object
			GameObject* go = new GameObject();//
			go->from_json(obj);
			// Insert new object into the map
			insert(go);
			if (go->getComponent<Components::PlayerInputPlatformer>()) {
				//insertClient(go); // Comment this out if using p2p
				if (uuid != m_playerID) {
					go->getComponent<Components::RigidBody>()->setIsKinematic(true);
					intSet.insert(go->getUUID());
				}
			}
		}
		else if (m_playerID != uuid) { // If it's an existing game object
			GameObject* go = m_objects->at(uuid);
			{
				std::lock_guard<std::mutex> lock(go->mutex);
				if ((networkType == 2 && !go->getComponent<Components::PlayerInputPlatformer>()) || networkType == 1) {
					go->from_json(obj);
				}
			}
		}
	}
	// Return the vector of new playerIDs
	return intSet;
}

/**
* Deserializes a string of gameObjects and inserts those GameObjects into the object map.
* This is meant to read in gameObject information sent from the server. This includes the player.
*
* @param gameObjectString: string containing movingObject information from the server
* @param networkType: defines the type of network being used (1=client2server, 2=peer2peer)
*/
void GameObjectManager::deserializeAll(std::string gameObjectString, int networkType) {
	// Parse json
	json j = json::parse(gameObjectString);

	// Loop through objects in JSON array
	for (const auto& obj : j) {
		auto uuid = obj["uuid"];

		if (!m_objects->count(uuid)) { // If it's a new game object
			GameObject* go = new GameObject();//
			go->from_json(obj);
			// Insert new object into the map
			insert(go);
			if (go->getComponent<Components::PlayerInputPlatformer>()) {
				//insertClient(go); // Comment this out if using p2p
				if (uuid != m_playerID) {
					go->getComponent<Components::RigidBody>()->setIsKinematic(true);
				}
			}
		}
		else { // If it's an existing game object
			GameObject* go = m_objects->at(uuid);
			{
				std::lock_guard<std::mutex> lock(go->mutex);
				if ((networkType == 2 && !go->getComponent<Components::PlayerInputPlatformer>()) || networkType == 1) {
					go->from_json(obj);
				}
			}
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
void GameObjectManager::deserializeClient(std::string gameObjectString, int networkType) {
	// TODO: Create new serialization function for MovingObjects once that's implemented into the Server
	json j = json::parse(gameObjectString);

	//std::cout << "JSON from deserialize: " << j.dump() << "\n";

	// Read in JSON array (should only be one object in peer to peer)
	//std::cout << "Game Object String: " << gameObjectString << "\n";
	int uuid = j["uuid"].get<int>();
	// Determine whether the object is new or existing
	try {
		if (!m_clientObjects->count(uuid)) { // If it's a new game object
			GameObject* go = new GameObject();
			go->from_json(j);
			// Insert new object into the map
			go->getComponent<Components::RigidBody>()->setIsKinematic(true);
			insertClient(go);
			insert(go);
		}
		else { // If it's an existing game object
			GameObject* go = m_clientObjects->at(uuid);
			go->from_json(j);
			if (go->getUUID() != m_playerID) {
				go->getComponent<Components::RigidBody>()->setIsKinematic(true);
			}
		}
	}
	catch (std::exception e) {
		//std::cout << e.what() << "\n";
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
	json j;

	for (const auto& [id, go] : *m_objects) {
		json gameObjectJson;
		{
			std::lock_guard<std::mutex> lock(go->mutex);
			go->to_json(gameObjectJson);
		}
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
* Returns a pointer to the client objects map
*/
std::map<int, GameObject*>* GameObjectManager::getClientObjectMap() {
	return m_clientObjects;
}

/**
* Returns the current Time of the timeline associated with gameObject calculations
*/
int64_t GameObjectManager::getCurrentTime() {
	return m_timeline->getTime();
}

/**
* Erases the object from the client map
*/
void GameObjectManager::terminateClient(int uuidKey) {
	// Find GameObject, then delete it
	if (GameObject *go = m_objects->at(uuidKey)) {
		m_objects->erase(uuidKey);
		m_clientObjects->erase(uuidKey);
		delete go;
	}	
	/*if (GameObject* go = m_clientObjects->at(uuidKey)) {
		
		delete go;
	}*/
}

/**
* Inserts the GameObject into the objects map
*
* @param go GameObject to be added to end of the object map
*/
void GameObjectManager::insert(GameObject* go) {
	// Sets UUID for the inserted object before adding it, if new
	if (go->getUUID() == 0) {
		m_idTracker--;
		go->setUUID(m_idTracker);
	}

	// TODO: It may be better to change so that it updates each of the components individually
	// Might also want to make it so that it doesn't try to load texture in the constructor so that
	// it doesn't call it every single time this function is called like last time

	// Adds or inserts existing information into the Manager
	m_objects->insert_or_assign(go->getUUID(), go);
}

/**
* Inserts the GameObject into the objects map
*
* @param go GameObject to be added to end of the object map
*/
void GameObjectManager::insertClient(GameObject* go) {
	// Adds or inserts existing information into the Manager
	m_clientObjects->insert_or_assign(go->getUUID(), go);
}

/**
* Finds a GameObject with the given idea, otherwise, returns null
*
* @param uuid: The ID of the object being serached for
* @returns A reference to the GameObject or a nullptr if nothing is found
*/
GameObject* GameObjectManager::find(int uuid) {
	try { // Try finding a GameObject in the objectMap
		return m_objects->at(uuid);
	}
	catch (std::out_of_range &e) { // If there is no GameObject at the given ID, return nullptr
		return nullptr;
	}
}

/**
* Attempts to get a reference to the PlayerGameObject given the playerID currently assigned in the
* system. If the player is not instantiated or does not exist, then returns a nullptr
*/
PlayerGO* GameObjectManager::tryGetPlayer() {
	try {
		return static_cast<PlayerGO*>(m_objects->at(m_playerID));
	}
	catch (std::out_of_range) {
		return nullptr;
	}
}

/**
* Gets the player ID
*/
int GameObjectManager::getPlayerID() {
	return m_playerID;
}

/**
* Sets the player ID so that the object with this ID will not be updated via JSON, only locally
*/
void GameObjectManager::setPlayerID(int uuid) {
	m_playerID = uuid;
}

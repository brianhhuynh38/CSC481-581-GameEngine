#pragma once

#ifndef GAMEOBJECTMANAGER_H
#define GAMEOBJECTMANAGER_H

#include "GameObject.h"
#include "timeline.h"

#include <map>

class GameObjectManager {
private:
	// Keeps track of the current ID and assigns it to added GameObjects
	int m_idTracker;
	// Map of GameObjects (Key: UUID, Value: GameObject)
	std::map<int, GameObject*> *m_objects;
	// Map of GameObjects controlled by the server
	std::map<int, GameObject*> *m_clientObjects;
	// The ID of the player under control of this client, maintained so that it is not updated externally
	int m_playerID;
	// Reference to Timeline for physics calculations
	Timeline* m_timeline;
public:

	/**
	* Constructor for the GameObjectManager that takes in a reference to the Timeline
	* 
	* @param timeline: Reference to the timeline
	*/
	GameObjectManager(Timeline *timelineRef);

	/**
	* Destructor that frees any allocated memory for the GameObjects
	*/
	~GameObjectManager();

	/**
	* Updates each of the GameObjects in the objects map
	*/
	void update();

	/**
	* Deserializes a string of gameObjects and inserts those GameObjects into the object map.
	* This is meant to read in gameObject information sent from the server
	*
	* @param movingEntityString: string containing movingObject information from the server
	* @param networkType: defines the type of network being used (1=client2server, 2=peer2peer)
	*/
	void deserialize(std::string gameObjectString, int networkType);

	/**
	* Deserializes a string of playerGO and inserts those GameObjects into the client object map.
	* This is meant to read in gameObject information sent from the server
	*
	* @param movingEntityString: string containing movingObject information from the server
	* @param networkType: defines the type of network being used (1=client2server, 2=peer2peer)
	*/
	void deserializeClient(std::string gameObjectString, int networkType);

	/**
	* Serializes a string of gameObjects
	*
	* @param gameObjectString: string containing movingObject information from the server
	* @param networkType: defines the type of network being used (1=client2server, 2=peer2peer)
	*/
	void serialize(std::string& outputString);

	/**
	* Returns the objects map
	*/
	std::map<int, GameObject*>* getObjectMap();

	/**
	* Returns the client objects map
	*/
	std::map<int, GameObject*>* getClientObjectMap();

	/**
	* Erases the object from the client map
	*/
	void terminateClient();

	/**
	* Inserts the GameObject into the objects map
	* 
	* @param go GameObject to be added to end of the object map
	*/
	void insert(GameObject* go);
	
	/**
	* Inserts the GameObject into the objects map
	*
	* @param go GameObject to be added to end of the object map
	*/
	void insertClient(GameObject* go);

	/**
	 * Sets the player ID so that the object with this ID will not be updated via JSON, only locally
	 */
	void setPlayerID(int uuid);

};

#endif

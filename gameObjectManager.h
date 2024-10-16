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
	std::map<int, GameObject>* m_serverObjects;
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
	* Deserializes a string of movingObjects and inserts those GameObjects into the object map
	*
	* @param movingEntityString: string containing movingObject information from the server
	* @param networkType: defines the type of network being used (1=client2server, 2=peer2peer)
	*/
	void serializeIn(std::string movingObjectString, int networkType);

	/**
	* Returns the objects map
	*/
	std::map<int, GameObject*>* getObjectMap();

	/**
	* Inserts the GameObject into the objects map
	* 
	* @param go GameObject to be added to end of the object map
	*/
	void insert(GameObject* go);
};

#endif

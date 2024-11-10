#include "moveObjectEvent.h"

#include "playerInput.h"

namespace Events {

	/**
	* Constructor for MoveObjectEvent for outbound events
	*/
	MoveObjectEvent::MoveObjectEvent(std::vector<GameObject*> goRef, int64_t timeStampPriority, int priority, zmq::socket_ref socketRef, int clientIdentifier) {
		// GameObject reference	
		m_goRefVector = goRef;
		// Event priorities
		m_timeStampPriority = timeStampPriority;
		m_priority = priority;
		// Socket refrence to send out information
		m_socketRef = socketRef;
		// Define client ID for message sending
		m_clientIdentifier = clientIdentifier;
		// Identifier false for onEvent function
		m_isReceiving = false;
		// Empty string since it's not relevant in a send until the end
		m_jsonString = "";
		m_goManagerRef = nullptr;
		// Assign reference to the clientIDQueue
		this->m_clientIDQueue = nullptr;
	}

	/**
	* Constructor for MoveObjectEvent for inbound Events
	*/
	MoveObjectEvent::MoveObjectEvent(GameObjectManager* goManager, int64_t timeStampPriority, int priority, std::string jsonString, std::queue<int>* clientIDQueue) {
		// GameObject reference
		m_goRefVector = std::vector<GameObject*>();
		// Event priorities
		m_timeStampPriority = timeStampPriority;
		m_priority = priority;
		// Socket is null because it is not needed for json parsing, client identifier is also set to 0 (invalid)
		m_socketRef = NULL;
		m_clientIdentifier = 0;
		// Identifier for the onEvent function
		m_isReceiving = true;
		// The jsonstring to be parsed
		m_jsonString = jsonString;
		// Set go manager reference 
		m_goManagerRef = goManager;
		// Assign reference to the clientIDQueue
		this->m_clientIDQueue = clientIDQueue;
	}

	void MoveObjectEvent::onEvent() {
		if (m_isReceiving) { // If this is receiving a JSON

			// Parse json
			json j = json::parse(m_jsonString);
			json gos = j["gos"];

			// Loops through all moving Objects
			if (gos.contains("moving")) {
				// Loop through objects in JSON array
				for (const auto& obj : gos["moving"]) {
					// Get the UUID of the object
					int uuid = obj["uuid"].get<int>();

					// Check if the Object exists
					if (GameObject* go = m_goManagerRef->find(uuid)) {
						// Get position values
						float x = obj["position"]["x"].get<float>();
						float y = obj["position"]["y"].get<float>();
						// Set transform position
						go->getComponent<Components::Transform>()->setPosition(x, y);
						go->getComponent <Components::RigidBody>()->setColliderCoordinates(x, y);
						// Set collider position
					}
				}
			}

			// Loops through player objects to check if they exist
			if (gos.contains("players")) {
				// Loop through objects in JSON array
				for (const auto& obj : gos["players"]) {
					// Get the UUID of the object
					int uuid = obj["uuid"].get<int>();

					// Check if the Object does not exist
					GameObject* go = m_goManagerRef->find(uuid);
					if (!go) {
						// Create a new GameObject and add it to the GameObjectManager
						go = new GameObject();
						go->from_json(obj);
						m_goManagerRef->insertClient(go);
						// Push the ID of the new client into the queue
						m_clientIDQueue->push(go->getUUID());
					}
				}
			}
		}
		else { // If this is sending out a JSON
			// Convert gameObject and Event data into json format
			json j;
			to_json(j);
			// If clientIdentifier is valid (not 0), then send clientIdentifier alongside JSON string
			std::string eventInfo = m_clientIdentifier != 0 ? "Client_" + std::to_string(m_clientIdentifier) + "\n" + j.dump() : j.dump();
			zmq::message_t msg(eventInfo);
			m_socketRef.send(msg, zmq::send_flags::dontwait);
		}
	}

	/*
	* Converts the Event to json for serialization across clients and server
	*/
	void MoveObjectEvent::to_json(json& j) const {
		// Add all fields to the json
		json gosJson;
		// Iterate through all of the GameObjects
		for (GameObject* go : m_goRefVector) {
			// JSON object to store positional information
			json gameObjectJson;
			
			// Push the JSON into the list of GameObjects
			// If player, send all info
			if (go->getComponent<Components::PlayerInputPlatformer>()) {
				go->to_json(gameObjectJson);
				// Push GameObject into the players list
				gosJson["players"].push_back(gameObjectJson);
			}
			else { // If movingObject, send positional info only
				// Set UUID
				gameObjectJson["uuid"] = go->getUUID();
				// Set Position
				Utils::Vector2D position = *go->getComponent<Components::Transform>()->getPosition();
				gameObjectJson["position"] = {
					{"x", position.x},
					{"y", position.y}
				};
				// Push GameObject in the list of MovingObjects
				gosJson["moving"].push_back(gameObjectJson);
			}
		}
		j["gos"] = gosJson;
		j["timeStampPriority"] = m_timeStampPriority;
		j["priority"] = m_priority;
	}

}
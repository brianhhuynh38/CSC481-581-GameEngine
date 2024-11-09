#include "moveObjectEvent.h"

#include "playerInput.h"

namespace Events {

	/**
	* Constructor for MoveObjectEvent for outbound events
	*/
	MoveObjectEvent::MoveObjectEvent(std::vector<GameObject*> goRef, int64_t timeStampPriority, int priority, zmq::socket_t* socket, int clientIdentifier) {
		// GameObject reference	
		m_goRefVector = goRef;
		// Event priorities
		m_timeStampPriority = timeStampPriority;
		m_priority = priority;
		// Socket refrence to send out information
		m_socketRef = socket;
		// Define client ID for message sending
		m_clientIdentifier = clientIdentifier;
		// Identifier false for onEvent function
		m_isReceiving = false;
		// Empty string since it's not relevant in a send until the end
		m_jsonString = "";
		m_goManagerRef = nullptr;
	}

	/**
	* Constructor for MoveObjectEvent for inbound Events
	*/
	MoveObjectEvent::MoveObjectEvent(GameObjectManager* goManager, int64_t timeStampPriority, int priority, std::string jsonString) {
		// GameObject reference
		m_goRefVector = std::vector<GameObject*>();
		// Event priorities
		m_timeStampPriority = timeStampPriority;
		m_priority = priority;
		// Socket is null because it is not needed for json parsing, client identifier is also set to 0 (invalid)
		m_socketRef = nullptr;
		m_clientIdentifier = 0;
		// Identifier for the onEvent function
		m_isReceiving = true;
		// The jsonstring to be parsed
		m_jsonString = jsonString;
		// Set go manager reference 
		m_goManagerRef = goManager;
	}

	void MoveObjectEvent::onEvent() const {
		if (m_isReceiving) { // If this is receiving a JSON

			// Parse json
			json j = json::parse(m_jsonString);

			// Loop through objects in JSON array
			for (const auto& obj : j["moving"]) {
				// Get the UUID of the object
				int uuid = obj["uuid"].get<int>();

				// Check if the Object exists
				if (GameObject* go = m_goManagerRef->find(uuid)) {
					// Set transform position
					go->getComponent<Components::Transform>()->setPosition(obj["position"]["x"].get<float>(), obj["position"]["y"].get<float>());
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
			m_socketRef->send(msg, zmq::send_flags::dontwait);
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
			// Set UUID
			gameObjectJson["uuid"] = go->getUUID();
			// Set Position
			Utils::Vector2D position = *go->getComponent<Components::Transform>()->getPosition();
			gameObjectJson["position"] = {
				{"x", position.x},
				{"y", position.y}
			};
			// Push the JSON into the list of GameObjects
			if (go->getComponent<Components::PlayerInputPlatformer>()) {
				// Push GameObject into the players list
				gosJson["players"].push_back(gameObjectJson);
			}
			else {
				// Push GameObject in the list of MovingObjects
				gosJson["moving"].push_back(gameObjectJson);
			}
		}
		j["gos"] = gosJson;
		j["timeStampPriority"] = m_timeStampPriority;
		j["priority"] = m_priority;
	}

}
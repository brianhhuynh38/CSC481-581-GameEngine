#include "playerUpdateEvent.h"

#include "vector2D.h"
#include "rigidBody.h"

namespace Events {

	PlayerUpdateEvent::PlayerUpdateEvent(std::vector<GameObject*> goRef, int64_t timeStampPriority, int priority, zmq::socket_t* sendSocket, int clientIdentifier) {
		// Register GameObjects into the vector
		this->m_goRefVector = goRef;
		// Set priorities
		this->m_timeStampPriority = timeStampPriority;
		this->m_priority = priority;
		// Define the socket that sends information out, clientIdentifier is 0 if invalid or unused
		this->m_socketRef = sendSocket;
		this->m_clientIdentifier = clientIdentifier;
		this->m_isReceiving = false;
		// JSON string and gameObject manager not needed for outbound
		this->m_jsonString = "";
		this->m_goManager = nullptr;
	}

	PlayerUpdateEvent::PlayerUpdateEvent(GameObjectManager* goManager, int64_t timeStampPriority, int priority, std::string jsonString) {
		// Instantiate empty GameObject vector
		m_goRefVector = std::vector<GameObject*>();
		// Set priorities
		this->m_timeStampPriority = timeStampPriority;
		this->m_priority = priority;
		// This is for receiving, so not socket is needed
		this->m_socketRef = nullptr;
		this->m_clientIdentifier = 0;
		this->m_isReceiving = true;
		// JSON string containing object info
		this->m_jsonString = jsonString;
		// Reference to the GameObjectManager
		this->m_goManager = goManager;
	}

	void PlayerUpdateEvent::onEvent() const {
			
		if (m_isReceiving) { // If this is receiving a JSON

			// Parse json
			json j = json::parse(m_jsonString);

			// Loop through objects in JSON array
			for (const auto& obj : j) {
				// Get the UUID of the object
				int uuid = obj["uuid"].get<int>();

				// Check if the Object exists
				if (GameObject* go = m_goManager->find(uuid)) {
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
			std::string eventInfo = m_clientIdentifier != 0 ? std::to_string(m_clientIdentifier) + "\n" + j.dump() : j.dump();
			zmq::message_t msg(eventInfo);
			m_socketRef->send(msg, zmq::send_flags::dontwait);
		}
	}

	void PlayerUpdateEvent::to_json(json& j) const {
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
			gosJson.push_back(gameObjectJson);
		}
		j["gos"] = gosJson;
		j["timeStampPriority"] = m_timeStampPriority;
		j["priority"] = m_priority;
	}

}
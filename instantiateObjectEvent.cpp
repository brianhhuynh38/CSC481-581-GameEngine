#include "instantiateObjectEvent.h"

#include "json.hpp"
using json = nlohmann::json;


namespace Events {

	// Constructor used for sending out Event information through a send socket and direct GameObject reference
	InstantiateObjectEvent::InstantiateObjectEvent(std::vector<GameObject*> goRef, int64_t timeStampPriority, int priority, zmq::socket_ref socket, int playerID, int clientIdentifier) {
		// GameObject reference	
		m_goRefVector = goRef;
		// Event priorities
		m_timeStampPriority = timeStampPriority;
		m_priority = priority;
		// Socket refrence to send out information
		m_socketRef = socket;
		// Define client ID for message sending
		m_clientIdentifier = clientIdentifier;
		m_playerID = playerID;
		// Identifier false for onEvent function
		m_isReceiving = false;
		// Empty string since it's not relevant in a send until the end
		m_jsonString = "";
		m_goManagerRef = nullptr;
	}

		// Constructor used for receiving Event information through an already-obtained message and direct GameObject reference
	InstantiateObjectEvent::InstantiateObjectEvent(GameObjectManager* goManager, int64_t timeStampPriority, int priority, std::string jsonString, int playerID) {
		// GameObject reference
		m_goRefVector = std::vector<GameObject*>();
		// Event priorities
		m_timeStampPriority = timeStampPriority;
		m_priority = priority;
		// Socket is null because it is not needed for json parsing, client identifier is also set to 0 (invalid)
		m_socketRef = NULL;
		m_clientIdentifier = 0;
		m_playerID = playerID;
		// Identifier for the onEvent function
		m_isReceiving = true;
		// The jsonstring to be parsed
		m_jsonString = jsonString;
		// Set go manager reference 
		m_goManagerRef = goManager;
	}

	void InstantiateObjectEvent::onEvent() {
		if (m_isReceiving) { // If the Event is currently receiving a message from another client
			// If this is a player instantiation from the server, define the player ID for the client
			if (m_playerID != 0) {
				m_goManagerRef->setPlayerID(m_playerID);
			}
			// Deserialize all GameObjects in the JSON after assigning playerID
			m_goManagerRef->deserialize(m_jsonString, 2);
			
			
		}
		else { // If the Event is currently sending out a message from this client
			// Convert gameObject and Event data into json format
			json j;
			to_json(j);
			// If clientIdentifier is valid (not 0), then send clientIdentifier alongside JSON string
			std::string eventInfo = m_clientIdentifier != 0 ? "Client_" + std::to_string(m_clientIdentifier) + "\n" + j.dump() : j.dump();
			zmq::message_t msg(eventInfo);
			m_socketRef.send(msg, zmq::send_flags::dontwait);
		}
	}

	void InstantiateObjectEvent::to_json(json& j) const {
		// Add all fields to the json
		json gosJson;

		for (GameObject* go : m_goRefVector) {
			json gameObjectJson;
			{
				std::lock_guard<std::mutex> lock(go->mutex);
				go->to_json(gameObjectJson);
			}
			gosJson.push_back(gameObjectJson);
		}
		j["gos"] = gosJson;
		j["timeStampPriority"] = m_timeStampPriority;
		j["priority"] = m_priority;
		if (m_playerID != 0) {
			j["playerid"] = m_playerID;
		}
	}
}

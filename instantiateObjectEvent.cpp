#include "instantiateObjectEvent.h"

#include "json.hpp"
using json = nlohmann::json;


namespace Events {

	// Constructor used for sending out Event information through a send socket and direct GameObject reference
	InstantiateObjectEvent::InstantiateObjectEvent(std::vector<GameObject*> goRef, int64_t timeStampPriority, int priority, zmq::socket_t* socket) {
		// GameObject reference	
		m_goRefVector = goRef;
		// Event priorities
		m_timeStampPriority = timeStampPriority;
		m_priority = priority;
		// Socket refrence to send out information
		m_socketRef = socket;
		// Identifier false for onEvent function
		m_isReceiving = false;
		// Empty string since it's not relevant in a send until the end
		m_jsonString = "";
		// Set go manager reference 
		m_goManagerRef = nullptr;
	}

		// Constructor used for receiving Event information through an already-obtained message and direct GameObject reference
	InstantiateObjectEvent::InstantiateObjectEvent(GameObjectManager* goManager, int64_t timeStampPriority, int priority, std::string jsonString) {
		// GameObject reference
		m_goRefVector = std::vector<GameObject*>();
		// Event priorities
		m_timeStampPriority = timeStampPriority;
		m_priority = priority;
		// Socket is null because it is not needed for json parsing
		m_socketRef = nullptr;
		// Identifier for the onEvent function
		m_isReceiving = true;
		// The jsonstring to be parsed
		m_jsonString = jsonString;
		// Set go manager reference 
		m_goManagerRef = nullptr;
	}

	void InstantiateObjectEvent::onEvent() const {
		if (m_isReceiving) { // If the Event is currently receiving a message from another client
			m_goManagerRef->deserialize(m_jsonString, 2);
		}
		else { // If the Event is currently sending out a message from this client
			json j;
			to_json(j);
			zmq::message_t msg(j.dump());
			m_socketRef->send(msg, zmq::send_flags::dontwait);
		}
	}

	void InstantiateObjectEvent::to_json(json& j) const {
		// Add all fields to the json, but records gameObject ID as 0 if there is no GameObject reference
		std::vector<int> idVector = std::vector<int>();
		json gosJson;

		for (GameObject* go : m_goRefVector) {
			json gameObjectJson;
			go->to_json(gameObjectJson);
			gosJson.push_back(gameObjectJson);
		}
		j["gos"] = gosJson;
		j["timeStampPriority"] = m_timeStampPriority;
		j["priority"] = m_priority;
	}
}

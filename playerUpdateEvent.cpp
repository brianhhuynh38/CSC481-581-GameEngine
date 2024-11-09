#include "playerUpdateEvent.h"

#include "vector2D.h"
#include "rigidBody.h"

namespace Events {

	PlayerUpdateEvent::PlayerUpdateEvent(std::vector<GameObject*> goRef, int64_t timeStampPriority, int priority, zmq::socket_t* sendSocket) {
		this->m_goRefVector = goRef;
		this->m_timeStampPriority = timeStampPriority;
		this->m_priority = priority;
		this->m_socketRef = sendSocket;
		this->m_isReceiving = false;
	}

	PlayerUpdateEvent::PlayerUpdateEvent(GameObjectManager* goManager, int64_t timeStampPriority, int priority, std::string jsonString) {
		// use ids to get goRefs
		//this->m_goRefVector = goRef;

		this->m_timeStampPriority = timeStampPriority;
		this->m_priority = priority;
		this->m_socketRef = nullptr;
		this->m_isReceiving = true;

		this->m_jsonString = jsonString;
		this->m_goManager = goManager;
	}

	void PlayerUpdateEvent::onEvent() const {
			
		if (m_isReceiving) { // receiving

			// Parse json
			json j = json::parse(m_jsonString);

			// Loop through objects in JSON array
			//for (const auto& obj : j) {
			//	auto uuid = obj["uuid"];

			//	if (uuid != m_playerID) { // If it's an existing game object
			//		GameObject* go = m_objects->at(uuid);
			//		if ((networkType == 2 && !go->getComponent<Components::PlayerInputPlatformer>()) || networkType == 1) {
			//			go->from_json(obj);
			//		}
			//	}
			//}
			//go->getComponent<Components::Transform>()->setPosition();
			//go->getComponent<Components::RigidBody>()->setColliderCoordinates();

			//// update each game object (player) with the given positional values
			//for (GameObject* go : m_goRefVector) {
			//	// TODO: Update GO information
			//	
			//}

		}
		else { // sending
				
			// update each game object (player) with the given positional values
			for (GameObject* go : m_goRefVector) {
				Components::Transform* transform = go->getComponent<Components::Transform>();
				Components::RigidBody* rb = go->getComponent<Components::RigidBody>();
				//transform->setPosition();
				//rb->setColliderCoordinates();
			}
		}
	}

	void PlayerUpdateEvent::to_json(json& j) const {
		
	}

}
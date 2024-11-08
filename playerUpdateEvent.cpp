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

		PlayerUpdateEvent::PlayerUpdateEvent(std::vector<int> ids, int64_t timeStampPriority, int priority, zmq::socket_t* receiveSocket) {
			// use ids to get goRefs
			//this->m_goRefVector = goRef;

			this->m_timeStampPriority = timeStampPriority;
			this->m_priority = priority;
			this->m_socketRef = receiveSocket;
			this->m_isReceiving = true;
		}

		void PlayerUpdateEvent::onEvent() const {
			
			if (m_isReceiving) { // receiving
				
				// update each game object (player) with the given positional values
				for (GameObject* go : m_goRefVector) {

				}

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

		void to_json(json& j) {
			
		}

}
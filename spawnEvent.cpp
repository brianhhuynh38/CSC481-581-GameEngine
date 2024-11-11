#include "spawnEvent.h"

#include "vector2D.h"
#include "transform.h"
#include "playerGO.h"

namespace Events {

	SpawnEvent::SpawnEvent(std::vector<GameObject*> goRefVector, int64_t timeStampPriority, int priority) {
		this->m_goRefVector = goRefVector;
		this->m_timeStampPriority = timeStampPriority;
		this->m_priority = priority;
	}

	void SpawnEvent::onEvent() {
		// Spawn each Player game object at its specified spawn point
		for (GameObject *go : m_goRefVector) {
			std::lock_guard<std::mutex> lock(go->mutex);
			PlayerGO* player = static_cast<PlayerGO*>(go);
			Utils::Vector2D spawnPos = *player->getSpawn()->getComponent<Components::Transform>()->getPosition();
			player->getComponent<Components::Transform>()->setPosition(spawnPos.x, spawnPos.y);
		}
	}

}

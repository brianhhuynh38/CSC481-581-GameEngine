#include "spawnEvent.h"

#include "vector2D.h"
#include "transform.h"
#include "playerGO.h"

namespace Events {

	void SpawnEvent::onEvent() const {
		// Spawn each Player game object at its specified spawn point
		for (GameObject *go : m_goRefVector) {
			PlayerGO* player = static_cast<PlayerGO*>(go);
			Utils::Vector2D spawnPos = *player->getSpawn()->getComponent<Components::Transform>()->getPosition();
			player->getComponent<Components::Transform>()->setPosition(spawnPos.x, spawnPos.y);
		}
	}

}

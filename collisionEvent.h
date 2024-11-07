#pragma once

#ifndef COLLISIONEVENT_H
#define COLLISIONEVENT_H

#include "event.h"

namespace Events {

	class CollisionEvent : public virtual Event {
	private:
		// A reference to the other object that the Object collided with
		GameObject* m_otherGORef;
	public:

		CollisionEvent(std::vector<GameObject*> goRef, int64_t timeStampPriority, int priority, GameObject* otherGORef);

		void onEvent() const override;
	};

}

#endif

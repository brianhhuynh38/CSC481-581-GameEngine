#pragma once

#ifndef COLLISIONEVENT_H
#define COLLISIONEVENT_H

#include "event.h"
#include "structs.h"

namespace Events {

	class CollisionEvent : public virtual Event {
	private:
		// Contains the information regarding the hit from the most recent collision check
		HitInfo* m_hitInfo;
	public:

		CollisionEvent(std::vector<GameObject*> goRef, int64_t timeStampPriority, int priority, HitInfo* hitInfo);

		void onEvent() const override;
	};

}

#endif

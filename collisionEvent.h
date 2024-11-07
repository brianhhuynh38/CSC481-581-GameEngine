#pragma once

#ifndef COLLISIONEVENT_H
#define COLLISIONEVENT_H

#include "event.h"

namespace Events {

	class CollisionEvent : public virtual Event {
	private:

	public:

		CollisionEvent(std::vector<GameObject*> goRef, int64_t timeStampPriority, int priority);

		void onEvent() const override;
	};

}

#endif

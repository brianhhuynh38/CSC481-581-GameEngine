#pragma once

#ifndef DEATHEVENT_H
#define DEATHEVENT_H

#include "event.h"

namespace Events {

	class DeathEvent : public virtual Event {
	public:

		DeathEvent(std::vector<GameObject*> goRef, int64_t timeStampPriority, int priority);

		void onEvent() override;
	};

}

#endif

#pragma once

#ifndef SPAWNEVENT_H
#define SPAWNEVENT_H

#include "event.h"

#include <vector>

namespace Events {

	class SpawnEvent : public virtual Event {
	public:

		SpawnEvent(std::vector<GameObject*> goRef, int64_t timeStampPriority, int priority);

		void onEvent() const override;
	};

}

#endif

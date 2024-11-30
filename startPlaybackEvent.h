#pragma once

#ifndef STARTPLAYBACKEVENT_H
#define STARTPLAYBACKEVENT_H

#include "event.h"

#include <vector>

namespace Events {

	class StartPlaybackEvent : public virtual Event {
	public:

		StartPlaybackEvent(int64_t timeStampPriority, int priority);

		void onEvent() override;
	};

}

#endif

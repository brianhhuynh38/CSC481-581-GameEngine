#pragma once

#ifndef STOPRECORDINGEVENT_H
#define STOPRECORDINGEVENT_H

#include "event.h"

#include <vector>

namespace Events {

	class StopRecordingEvent : public virtual Event {
	public:

		StopRecordingEvent(int64_t timeStampPriority, int priority);

		void onEvent() override;
	};

}

#endif

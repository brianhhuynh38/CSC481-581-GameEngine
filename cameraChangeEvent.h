#pragma once

#ifndef CAMERACHANGEEVENT_H
#define CAMERACHANGEEVENT_H

#include "event.h"

#include <vector>

namespace Events {

	class CameraChangeEvent : public virtual Event {
	public:

		CameraChangeEvent(std::vector<GameObject*> goRef, int64_t timeStampPriority, int priority);

		void onEvent() override;
	};

}

#endif

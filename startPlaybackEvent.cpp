#include "startPlaybackEvent.h"

#include "vector2D.h"
#include "transform.h"
#include "playerGO.h"
#include "boundaryZone.h"
#include "global.h"

#include <iostream>

namespace Events {

	StartPlaybackEvent::StartPlaybackEvent(int64_t timeStampPriority, int priority) {
		this->m_timeStampPriority = timeStampPriority;
		this->m_priority = priority;
	}

	void StartPlaybackEvent::onEvent() {
		// Start replaying the recording from the queue of events and their timestamps
		if (!startPlayback) {
			std::cout << "Playback Starting...\n";
			isRecording = false;
			startPlayback = true;
		}
	}
}

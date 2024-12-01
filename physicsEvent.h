#pragma once

#pragma once

#ifndef PHYSICSEVENT_H
#define PHYSICSEVENT_H

#include "event.h"
#include "GameObject.h"


namespace Events {

	class PhysicsEvent : public virtual Event {
	private:
		
	public:
		
		/**
		* Constructor for PhysicsEvent
		*/
		PhysicsEvent(std::vector<GameObject*> goRef, int64_t timeStampPriority, int priority);

		/**
		* Calculates and updates all physics vectors
		*/
		void onEvent() override;
	};
}



#endif

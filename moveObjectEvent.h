#pragma once

#ifndef MOVEOBJECTEVENT_H
#define MOVEOBJECTEVENT_H


#include "event.h"
#include "vector2D.h"
#include "GameObject.h"

#include <zmq.hpp>

namespace Events {
	class MoveObjectEvent : public virtual Event {
	private:

	public:
		
		/**
		* Constructor for MoveObjectEvent
		*/
		MoveObjectEvent(std::vector<GameObject*> goRef, int64_t timeStampPriority, int priority, zmq::socket_t* socket);

		void onEvent() const override;

	};

}


#endif

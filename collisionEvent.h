#pragma once

#ifndef COLLISIONEVENT_H
#define COLLISIONEVENT_H

#include "event.h"

namespace Events {

	class CollisionEvent : public virtual Event {
	private:

	public:
		void onEvent() const override;
	};

}

#endif

#pragma once

#ifndef DEATHEVENT_H
#define DEATHEVENT_H

#include "event.h"

namespace Events {

	class DeathEvent : public virtual Event {
	public:
		void onEvent() const override;
	};

}

#endif

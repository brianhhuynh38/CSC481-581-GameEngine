#pragma once

#ifndef SPAWNEVENT_H
#define SPAWNEVENT_H

#include "event.h"

namespace Events {

	class SpawnEvent : public virtual Event {
	public:
		void onEvent() const override;
	};

}

#endif

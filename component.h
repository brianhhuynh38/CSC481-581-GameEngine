#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

namespace Component {

	class Component {
	public:
		virtual ~Component() {}
		virtual void update() = 0;
	};

}

#endif

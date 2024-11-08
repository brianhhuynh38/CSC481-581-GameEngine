#pragma once

#ifndef PLAYERUPDATEEVENT_H
#define PLAYERUPDATEEVENT_H

#include "event.h"
#include "GameObject.h"

#include <zmq.hpp>

namespace Events {

	/**
	* Updates the positional values of the target player game object.
	*/
	class PlayerUpdateEvent : public virtual Event {
	private:
		// true if this event is being received by another client
		bool m_isReceiving;
		// socket reference for sending/receiving
		zmq::socket_t* m_socketRef;

	public:

		/**
		* constructor for PlayerUpdateEvent
		*/
		PlayerUpdateEvent(std::vector<GameObject*> goRef, int64_t timeStampPriority, int priority, zmq::socket_t* sendSocket);

		/**
		* constructor for PlayerUpdateEvent
		*/
		PlayerUpdateEvent(std::vector<int> ids, int64_t timeStampPriority, int priority, zmq::socket_t* receiveSocket);

		/**
		* function called when event is called
		*/
		void onEvent() const override;

		void to_json(json& j) override;
	};

}

#endif

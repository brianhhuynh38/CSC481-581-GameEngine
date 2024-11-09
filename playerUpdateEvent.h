#pragma once

#ifndef PLAYERUPDATEEVENT_H
#define PLAYERUPDATEEVENT_H

#include "event.h"
#include "GameObject.h"
#include "gameObjectManager.h"

#include <zmq.hpp>

namespace Events {

	/**
	* Updates the positional values of the target player game object.
	*/
	class PlayerUpdateEvent : public virtual Event {
	private:
		// true if this event is being received by another client
		bool m_isReceiving;
		// The client ID used to filter messages, this will not be used if it is passed in as 0
		int m_clientIdentifier;
		// socket reference for sending/receiving
		zmq::socket_t* m_socketRef;
		// JSON string containing positional update information for object
		std::string m_jsonString;

		// Reference to GameObjectManager
		GameObjectManager* m_goManager;

	public:

		/**
		* constructor for PlayerUpdateEvent for outbound events
		*/
		PlayerUpdateEvent(std::vector<GameObject*> goRef, int64_t timeStampPriority, int priority, zmq::socket_t* sendSocket, int clientIdentifier=0);

		/**
		* constructor for PlayerUpdateEvent for inbound events
		*/
		PlayerUpdateEvent(GameObjectManager* goManager, int64_t timeStampPriority, int priority, std::string jsonString);

		/**
		* function called when event is called
		*/
		void onEvent() const override;

		void to_json(json& j) const override;
	};

}

#endif

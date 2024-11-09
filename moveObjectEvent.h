#pragma once

#ifndef MOVEOBJECTEVENT_H
#define MOVEOBJECTEVENT_H


#include "event.h"
#include "GameObject.h"
#include "gameObjectManager.h"

#include <zmq.hpp>

namespace Events {
	class MoveObjectEvent : public virtual Event {
	private:
		// Whether or not the Event is receiving or sending out data
		bool m_isReceiving;
		// The client ID used to filter messages, this will not be used if it is passed in as 0
		int m_clientIdentifier;
		// Reference to the socket used for sending information out
		zmq::socket_t* m_socketRef;
		// JSON string to parse with Object Information in it
		std::string m_jsonString;

		// Reference to the GameObjectManager
		GameObjectManager* m_goManagerRef;

	public:
		
		/**
		* Constructor for MoveObjectEvent for outbound events
		*/
		MoveObjectEvent(std::vector<GameObject*> goRef, int64_t timeStampPriority, int priority, zmq::socket_t* socket, int clientIdentifier = 0);

		/**
		* Constructor for MoveObjectEvent for inbound Events
		*/
		MoveObjectEvent(GameObjectManager* goManager, int64_t timeStampPriority, int priority, std::string jsonString);

		/**
		* The function that parses or sends out information on positional data changes
		*/
		void onEvent() const override;

		/*
		* Converts the Event to json for serialization across clients and server
		*/
		void to_json(json& j) const override;

	};

}


#endif

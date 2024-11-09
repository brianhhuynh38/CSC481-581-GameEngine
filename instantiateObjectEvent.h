#pragma once

#ifndef INSTANTIATEOBJECTEVENT_H
#define INSTANTIATEOBJECTEVENT_H

#include "event.h"
#include "gameObjectManager.h"

#include <zmq.hpp>
#include <string>

namespace Events {

	class InstantiateObjectEvent : public virtual Event {
	private:

		// Whether or not the Event is receiving or sending out data
		bool m_isReceiving;
		// The player ID for the player being instantiated if applicable, unused if 0
		int m_playerID;
		// The client ID used to filter messages, this will not be used if it is passed in as 0
		int m_clientIdentifier;
		// A reference to the socket being used for a particular data transfer
		zmq::socket_t* m_socketRef;
		// JSON string to parse with Object Information in it
		std::string m_jsonString;

		// Reference to the GameObjectManager
		GameObjectManager* m_goManagerRef;

	public:

		// Constructor used for sending out Event information through a send socket and direct GameObject reference
		InstantiateObjectEvent(std::vector<GameObject*> goRef, int64_t timeStampPriority, int priority, zmq::socket_t* socket, int playerID = 0, int clientIdentifier = 0);

		// Constructor used for receiving Event information through an already-obtained message and direct GameObject reference
		InstantiateObjectEvent(GameObjectManager* goManager, int64_t timeStampPriority, int priority, std::string jsonString, int playerID = 0);
		
		// Overridden onEvent function that parses or sends out a message
		void onEvent() const override;

		// Converts the Event to json for serialization across clients and server
		void to_json(json& j) const override;

	};

}

#endif

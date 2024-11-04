#include "eventManager.h"

/**
	* Constructor for EventManager that intializes the event queue
	*/
EventManager::EventManager() {
	eventQueue = std::priority_queue<Events::Event>();
}

/**
* Destructor for EventManager that deletes each Event stored in the manager
*/
EventManager::~EventManager() {

}

/**
* Registers an Event into the
*/
void EventManager::registerEvent(Events::Event event) {
	
}

void EventManager::raiseEvent(Events::Event event) {

}

void EventManager::dispatchEvent() {

}


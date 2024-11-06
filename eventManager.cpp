#include "eventManager.h"

/**
	* Constructor for EventManager that intializes the event queue
	*/
EventManager::EventManager() {
	m_eventQueue = std::priority_queue<Events::Event>();
	m_eventRegistry = std::map<std::type_index, std::vector<GameObject*>>();
}

/**
* Destructor for EventManager that deletes each Event stored in the manager
*/
EventManager::~EventManager() {

}

/**
* Registers an Event into the event map and assigns the GameObjects to the Event
*/
template <typename E>
void EventManager::registerEvent(GameObject *gameObject) {
	// Try to push back GameObject; insert new entry if Event doesn't exist yet
	try {
		m_eventRegistry.at(typeid(E)).push_back(gameObject);
	}
	catch (std::out_of_range) {
		m_eventRegistry.insert_or_assign(typeid(E), std::vector<GameObject*>());
		m_eventRegistry.at(typeid(E)).push_back(gameObject);
	}
}

void EventManager::raiseEvent(Events::Event &event) {
	m_eventQueue.push(event);
}

void EventManager::dispatchEvents(int64_t timeStamp) {
	// Return if the event queue is empty
	if (m_eventQueue.empty()) {
		return;
	}
	
	// Keep dispatching events until the priority is higher than the given timeStamp
	while (m_eventQueue.top().getTimeStampPriority() <= timeStamp) {
		m_goManager->receiveEvent(m_eventQueue.top());
		m_eventQueue.pop();
	}
}

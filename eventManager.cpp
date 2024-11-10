#include "eventManager.h"

#include "gameObject.h"

#include "event.h"
#include "collisionEvent.h"
#include "deathEvent.h"
#include "inputEvent.h"
#include "spawnEvent.h"


/**
* Constructor for EventManager that intializes the event queue
*/
EventManager::EventManager() {
	m_eventQueue = std::priority_queue<Events::Event*, std::vector<Events::Event*>, std::greater<Events::Event*>>();
	m_eventRegistry = std::map<std::type_index, std::vector<GameObject*>>();
}

/**
* Destructor for EventManager that deletes each Event stored in the manager
*/
EventManager::~EventManager() { }

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

/**
* Adds the given event into the event queue
* @param Event to add
*/
void EventManager::raiseEvent(Events::Event* event) {
	std::shared_lock<std::shared_mutex> queueLock(m_mutex);
	m_eventQueue.push(event);
}

/**
  * Instantly raises an Event and activates it, skips queue (primarily to preserve multi-threaded nature of networked events, maybe temp)
  * @param Event to activate
  */
void EventManager::raiseEventInstantly(Events::Event* event) {
	event->onEvent();
}

/**
* Adds the given event into the event queue
* @param timeStampPriority: The time that the event should be executed
* @param priority: The priority of each event outside of the time priority
*/
template <typename E>
void EventManager::raiseEventAll(int64_t timeStampPriority, int priority) {
	//std::vector<GameObject*> gos;
	//try {
	//	gos = m_eventRegistry.at(typeid(E));
	//}
	//catch (std::out_of_range) {
	//	std::cerr << "Event registry accessed by nonexistent Event type";
	//	return;
	//}
	//switch (typeid(E)) {
	//	case typeid(Events::CollisionEvent) :
	//	{
	//		//raiseEvent(new Events::CollisionEvent());
	//		// This probably shouldn't be raised globally
	//		break;
	//	}
	//	case typeid(Events::DeathEvent) :
	//	{
	//		raiseEvent(new Events::DeathEvent(gos, timeStampPriority, priority));
	//		break;
	//	}
	//	case typeid(Events::InputEvent) :
	//	{
	//		// Debating whether this should be implemented
	//		break;
	//	}
	//	case typeid(Events::SpawnEvent) :
	//	{
	//		raiseEvent(new Events::SpawnEvent(gos, timeStampPriority, priority));
	//		break;
	//	}
	//	default:
	//	{
	//		std::cout << "Invalid Event type used for RaiseEventAll()\n";
	//		break;
	//	}
	//}
}

/**
* Private helper function that returns a copy of the top of the event queue
*/
Events::Event* EventManager::getEventQueueTop() const {
	return m_eventQueue.top();
}

/**
* Dispatches any events in the priority queue which
* meet the time and priority requirements
*/
void EventManager::dispatchEvents(int64_t timeStamp) {
	// Lock mutex
	std::shared_lock<std::shared_mutex> queueLock(m_mutex);

	// Return if the event queue is empty
	if (m_eventQueue.empty()) {
		return;
	}
		
	// Keep dispatching events until the priority is higher than the given timeStamp
	while (!m_eventQueue.empty() && m_eventQueue.top()->getTimeStampPriority() <= timeStamp) {
		// Get event at the front
		Events::Event* e = getEventQueueTop();
		// Run the onEvent, then delete
		e->onEvent();
		//delete e;
		// Remove the Event from the queue
		m_eventQueue.pop();
	}
}

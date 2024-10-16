//#include "GameObject.h"
//#include "component.h"
//#include "transform.h"
//#include "textureMesh.h"
//#include "playerInput.h"
//#include "rigidBody.h"
//#include "timeline.h"
//#include "definitions.h"
//
//#include <iostream>
//#include <map>
//#include <vector>
//#include <memory>
//#include <typeindex>
//
///*
//* Some references include: https://www.gamedeveloper.com/design/the-entity-component-system---an-awesome-game-design-pattern-in-c-part-1-
//* http://entity-systems.wikidot.com/test-for-parallel-processing-of-components#cpp
//* Textbook
//*/
//
//// Default constructor for GameObject. Adds a Transform component (required)
//GameObject::GameObject(){
//	//addComponent<Components::Transform>(1, 1, 0, 0, 1, 1);
//}
//
//// Constructor with fields for GameObject. Adds a Transform component (required)
//GameObject::GameObject(float scaleX, float scaleY, float positionX, float positionY, float width, float height) {
//	//addComponent<Components::Transform>(scaleX, scaleY, positionX, positionY, width, height);
//}
//
//// Function to add component to the game object
//template<typename T, typename... Args>
//void GameObject::addComponent(Args&&... args) {
//	// Create a unique_ptr for the component and store it using its typeid
//	m_components[typeid(T)] = std::make_unique<T>(std::forward<Args>(args)...);
//}
//
///**
//* Uses the type given in the brackets to determine the component to get, then returns it:
//* Usage: getComponent<TYPE>()
//* 
//* @return Returns the component specified by the template type
//*/
//template<typename T>
//T* GameObject::getComponent() {
//	auto it = m_components.find(typeid(T));
//	if (it != m_components.end()) {
//		return static_cast<T*>(it->second.get());  // Safely cast the component to its type
//	}
//	return nullptr;
//}
//
//// Update all components attached to game object
//void GameObject::update() {
//	for (auto& [type, component] : m_components) {
//		component->update();  // Update each component
//	}
//}
//
//void GameObject::setUUID(int uuid) {
//	this->m_uuid = uuid;
//}
//
//int GameObject::getUUID() {
//	return m_uuid;
//}
//
//double GameObject::getDeltaTimeInSecsOfObject() {
//	return m_timeline->getDeltaTime() / MICROSEC_PER_SEC;
//}
//
//// Example for main:
//// Player player(1, 1, 0, 0, 50, 50, 10, "player_texture.png", false, true, 0, 10, 5.0f);
//// StaticPlatform platform(1, 1, 100, 50, 200, 10, 100, "platform_texture.png", true, false);
//
//// Others to add:
//// - MovingPlatform: Server-side.  TextureMesh, RigidBody, MovementController?
//// - SpawnPoint: DON'T need to add, can just be a GameObject, only needs Transform
//// - DeathZone: RigidBody (for collisions)
//// - SideBoundary: RigidBody (for collisions)

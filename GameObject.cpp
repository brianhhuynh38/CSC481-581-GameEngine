#include "GameObject.h"
#include <iostream>
#include <vector>
#include <memory>

/*
* Some references include: https://www.gamedeveloper.com/design/the-entity-component-system---an-awesome-game-design-pattern-in-c-part-1-
* http://entity-systems.wikidot.com/test-for-parallel-processing-of-components#cpp
*/
class GameObject {
public:
	// Function to add component to the game object
	template<typename T, typename... Args>
	void addComponent(Args&&... args) {
		components.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
		components[typeid(T)] = std::move(component);
	}

	template<typename T>
	T* getComponent() {
		auto it = components.find(typeid(T));
		if (it != components.end()) {
			return static_cast<T*>(it->second.get());
		}
	}

	// Update all components attached to game object
	void update() {
		for (auto& component : components) {
			component->update(*this);
		}
	}

	// Game Object Properties (not sure if we need this here? maybe in our EntityComponent though)
	float x = 0, y = 0; // Global position (not based on display)
	float vx = 0, vy = 0; // Global velocity
	// Add other relevant data here

private:
	std::vector<std::unique_ptr<Component>> components; // Container for all components
};



// JUST EXAMPLE CODE FOR HOW A COMPONENT COULD WORK (based on my understanding):
// 
// Class handling movement of component:
// class TransformComponent : public Component {
//public:
//	TransformComponent(float x, float y) : startX(x), startY(y) {}
//
//	void update(GameObject& obj) override {
//		obj.x = startX;
//		obj.y = startY;
//		std::cout << "Transform Component: (" << obj.x << ", " << obj.y << ")\n";
//	}
//
//private:
//	float startX, startY;
//};
// 
// Uses TransformComponent and applies physics:
//class PhysicsComponent : public Component {
//public:
//	PhysicsComponent(float gravity) : gravity(gravity) {}
//
//	void update(GameObject& obj, Timeline* timeline) override {
//		auto transform = obj.getComponent<TransformComponent>();
//		if (transform) {
//			// Apply physics to the transform component
//			transform->y += gravity * timeline->getDeltaTime();
//		}
//	}
//
//private:
//	float gravity;
//};

#include "GameObject.h"
#include "component.h"
#include "transform.h"
#include <iostream>
#include <map>
#include <vector>
#include <memory>

/*
* Some references include: https://www.gamedeveloper.com/design/the-entity-component-system---an-awesome-game-design-pattern-in-c-part-1-
* http://entity-systems.wikidot.com/test-for-parallel-processing-of-components#cpp
*/
class GameObject {
protected:
	std::vector<std::unique_ptr<Component::Component>> components; // Container for all components
public:
	GameObject(float scaleX, float scaleY, float positionX, float positionY, float width, float height) {
		this->addComponent(new Component::Transform(scaleX, scaleY, positionX, positionY, width, height));
	}

	// Function to add component to the game object
	template<typename T, typename... Args>
	void addComponent(Args&&... args) {
		components.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
		components[typeid(T)] = std::move(component);
	}

	/**
	* Uses the type given in the brackets to determine the component to get, then returns it:
	* Usage: getComponent<TYPE>()
	* 
	* @return Returns the component specified by the template type
	*/
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
			component->update();
		}
	}
};
class Player : public GameObject {
protected:
	// Add player property fields here
public:
	Player(float scaleX, float scaleY, float positionX, float positionY, float width, float height, float mass,
		std::string textureFilepath, bool isStationary, bool affectedByPhysics, float jumpVectorX, float jumpVectorY,
		float maxSpeed) {

	}


}

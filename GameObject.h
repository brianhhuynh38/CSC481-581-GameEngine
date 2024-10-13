#pragma once
class GameObject; // Forward declaration

class Component {
public:
	virtual void update(GameObject& obj) = 0; // Function for updating the component
	virtual ~Component() = default; // Component destructor for deletion
};

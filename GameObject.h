#pragma once

#include "component.h"
#include "transform.h"
#include "textureMesh.h"
#include "playerInput.h"
#include "rigidBody.h"
#include "timeline.h"

#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include <typeindex>


#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

/*
* Some references include: https://www.gamedeveloper.com/design/the-entity-component-system---an-awesome-game-design-pattern-in-c-part-1-
* http://entity-systems.wikidot.com/test-for-parallel-processing-of-components#cpp
* Textbook
*/
class GameObject {
protected:
    // Store components in a map using std::type_index as the key
    std::map<std::type_index, std::unique_ptr<Components::Component>> m_components;
    // Reference pointer to the timeline controlling GameObject behaviors
    Timeline* m_timeline;
    // UUID
    int m_uuid;
    // Put stuff in struct?
public:
    // Default constructor for GameObject. Adds a Transform component (required)
    GameObject();

    // Constructor with fields for GameObject. Adds a Transform component (required)
    GameObject(float scaleX, float scaleY, float positionX, float positionY, float width, float height);

    // Function to add component to the game object
    template<typename T, typename... Args>
    void addComponent(Args&&... args);

    /**
    * Uses the type given in the brackets to determine the component to get, then returns it:
    * Usage: getComponent<TYPE>()
    *
    * @return Returns the component specified by the template type
    */
    template<typename T>
    T* getComponent();

    // Update all components attached to game object
    void update();

    void setUUID(int uuid);

    int getUUID();

    double getDeltaTimeInSecsOfObject();
};


// Example for main:
// Player player(1, 1, 0, 0, 50, 50, 10, "player_texture.png", false, true, 0, 10, 5.0f);
// StaticPlatform platform(1, 1, 100, 50, 200, 10, 100, "platform_texture.png", true, false);

// Others to add:
// - MovingPlatform: Server-side.  TextureMesh, RigidBody, MovementController?
// - SpawnPoint: DON'T need to add, can just be a GameObject, only needs Transform
// - DeathZone: RigidBody (for collisions)
// - SideBoundary: RigidBody (for collisions)

#endif

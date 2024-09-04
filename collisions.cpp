#include "collisions.h"

/**
    * Checks collisions
    * This was referenced from the SDL2 tutorial:
    * https://lazyfoo.net/tutorials/SDL/27_collision_detection/index.php
    */
bool checkCollision(SDL_Rect a, SDL_Rect b)
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if (bottomA <= topB)
    {
        return false;
    }

    if (topA >= bottomB)
    {
        return false;
    }

    if (rightA <= leftB)
    {
        return false;
    }

    if (leftA >= rightB)
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

/**
    * Loop through entities, use their colliders to check the collisions, using checkCollision method
    * This was referenced from the SDL2 tutorial:
    * https://lazyfoo.net/tutorials/SDL/27_collision_detection/index.php
    */
bool checkCollisions(std::list<SDL_Rect> collisions, std::list<Entities::Entity> entities) {
    // Loop through entities, use their colliders to check the collisions, using checkCollision method
    for (auto& entity : entities) { // Loop through each entity
        for (auto& entityCollider : entity.getColliders()) { // Loop through each entity's collider
            for (auto& colliderRect : collisions) { // Loop through the colliders list given
                if (SDL_HasIntersection(&entityCollider, &colliderRect)) {
                    return true;
                }
            }
        }
    }
    
    return false;
}

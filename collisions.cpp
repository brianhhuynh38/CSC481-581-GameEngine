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
HitInfo checkCollisions(std::list<SDL_Rect> *collisions, std::map<int, Entities::Entity> *entities) {
    // Create list iterator
	std::map<int, Entities::Entity>::iterator iterEnt;

    // Create hit responce for return
    HitInfo hInfo{false};

    // Loop through entities, use their colliders to check the collisions, using checkCollision method
    for (iterEnt = entities->begin(); iterEnt != entities->end(); ++iterEnt) { // Iterate through all entities

        // Create colliders iterator
        std::list<SDL_Rect>::iterator iterCol;

        // Get colliders of entity
        std::list<SDL_Rect> *entityCollisions = iterEnt->second.getColliders();

        // Loop through colliders from 'entities'
        for (iterCol = entityCollisions->begin(); iterCol != entityCollisions->end(); ++iterCol) { // Iterate through all of the colliders of the entity

            // Create colliders iterator of other objects
            std::list<SDL_Rect>::iterator iterColOther;

            // Get colliders of entity to check against checked object
            std::list<SDL_Rect> *entityCollisions = iterEnt->second.getColliders();

            for (iterColOther = collisions->begin(); iterColOther != collisions->end(); ++iterColOther) { // Iterate through all of the colliders of the entity
                if (&(*iterCol) != &(*iterColOther)) {
                    //std::cout << "Collider 1: " << iterCol->x << "," << iterCol->y << "," << iterCol->w << "," << iterCol->h << "\n";
                    //std::cout << "Collider 2: " << iterColOther->x << "," << iterColOther->y << "," << iterColOther->w << "," << iterColOther->h << "\n";
                }
                if ((&(*iterCol) != &(*iterColOther)) && SDL_HasIntersection(&(*iterCol), &(*iterColOther))) {
                    
                    Utils::Vector2D direction = Utils::Vector2D(iterColOther->x, iterColOther->y).add(Utils::Vector2D(-iterCol->x, -iterCol->y));
                    
                    hInfo.hit = true;
                    hInfo.hitVector = direction;
                    return hInfo;
                }
            }
        }
    }
    
    return hInfo;
}

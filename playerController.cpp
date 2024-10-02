#include <map>

#include "playerController.h"

#include "global.h"
#include "SDL.h"
#include "collisions.h"
#include "physics.h"
#include "structs.h"
#include "input.h"

namespace Controllers {
    
    /**
     * Constructor for PlayerController
     * @param *player pointer to player to control
     */
    PlayerController::PlayerController(Entities::Player *player, EntityController *entityController){
        this->player = player;
        this->entityController = entityController;
    }

    void PlayerController::updatePlayerPhysics(Timeline *timeline, Physics *physics) {
        physics->updateEntityPhysicsVectors(timeline, player);
    }

    /**
     * Checks for movement inputs from keys and applies movement to player
     */
    void PlayerController::movementInput(Timeline *timeline, InputHandler *inputHandler) {
        // gets maxSpeed from player
        float speed = player->getMaxSpeed();
        Utils::Vector2D moveVector = Utils::Vector2D(0,0);
        moveVector.y = player->getAcceleration()->y;

        // horizontal movement
        if (inputHandler->keyboard[SDL_SCANCODE_RIGHT] == 1) {
            if (useAcceleration) {
                moveVector.x += speed;
            }
            else {
                moveVector.x = speed;
            }
        }
        if (inputHandler->keyboard[SDL_SCANCODE_LEFT] == 1) {
            if (useAcceleration) {
                moveVector.x -= speed;
            }
            else {
                moveVector.x = -speed;
            }
        }

        // vertical movement (platformer)
        if (platformerMovement) {
            if (inputHandler->keyboard[SDL_SCANCODE_DOWN] == 1) {

            }
            // Jump
            if (inputHandler->keyboard[SDL_SCANCODE_UP] == 1 && player->getIsGrounded()) {
                moveVector.y = player->getJumpVector()->y;
                player->setIsGrounded(false);
            }
        }
        else {
            // vertical movement (top down)
            if (inputHandler->keyboard[SDL_SCANCODE_DOWN] == 1) {
                if (useAcceleration) {
                    moveVector.y += speed;
                }
                else {
                    moveVector.y = speed;
                }
            }
            if (inputHandler->keyboard[SDL_SCANCODE_UP] == 1) {
                if (useAcceleration) {
                    moveVector.y -= speed;
                }
                else {
                    moveVector.y = -speed;
                }
            }
        }

        // if moving diagonally, multiplies vectors xy values by cos(45deg)
        if (!platformerMovement && moveVector.x != 0 && moveVector.y != 0) {
            moveVector.x *= 0.525322;
            moveVector.y *= 0.525322;
        }
        moveVector.multConst(timeline->getDeltaTime() / MICROSEC_PER_SEC);

        player->updateVelocity(moveVector);


        //std::cout << "moveVector: " << moveVector.x << ", " << moveVector.y << "\n";

        // move on x axis
        movePlayer(Utils::Vector2D(moveVector.x, NULL), timeline);
        // move on y axis
        movePlayer(Utils::Vector2D(NULL, moveVector.y), timeline);
        
    }

    /**
     * Moves the player by a given amount by manipulating its velocity vector
     * @param movementVector The vector that is changes the player's movement
     */
    void PlayerController::movePlayer(Utils::Vector2D movementVector, Timeline *timeline) {
        Utils::Vector2D moveVector;
        // quick fix for geting to fast 

        float maxMove = 10;
        if (movementVector.x != NULL) {
            if (movementVector.x > maxMove) {
                movementVector.x = maxMove;
            }
            else if (movementVector.x < -maxMove) {
                movementVector.x = -maxMove;
            }
            moveVector = Utils::Vector2D(movementVector.x, 0);
        }
        else if (movementVector.y != NULL) {
            if (movementVector.y > maxMove) {
                movementVector.y = maxMove;
            }
            else if (movementVector.y < -maxMove) {
                movementVector.y = -maxMove;
            }
            moveVector = Utils::Vector2D(0, movementVector.y);
        }

        // update velocity
        //*m_acceleration = moveVector;
        player->updateVelocity(moveVector);
        player->updatePosition(moveVector);

        // Create colliders iterator
        std::list<SDL_Rect>::iterator iterCol;

        std::map<int, Entities::Entity> entityMap = *entityController->getEntities();

        //std::cout << "Player coordinates: " << m_position->x << ", " << m_position->y << "\n";
        // Loop through colliders
        for (iterCol = player->getColliders()->begin(); iterCol != player->getColliders()->end(); ++iterCol) {
            // Move the colliders the calculated distance
            iterCol->x = player->getPosition()->x;
            iterCol->y = player->getPosition()->y;
            //std::cout << "Collider coordinates: " << iterCol->x << "," << iterCol->y << "\n";
        }

        // Checks for collisions
        HitInfo hInfo = checkCollisions(player->getColliders(), &entityMap); 

        // If the object collided with something
        if (hInfo.hit) {
            if (movementVector.x != NULL) { // x-axis collision
                //std::cout << "X-HIT\n";
                player->updatePosition(moveVector.multConst(-1));
                player->setVelocity(0, player->getVelocity()->x);
            }
            if (movementVector.y != NULL) { // y-axis collision
                //std::cout << "Y-HIT\n";
                player->updatePosition(Utils::Vector2D(0, player->getVelocity()->multConst(-1 * (timeline->getDeltaTime() / MICROSEC_PER_SEC)).y - moveVector.y));
                player->setVelocity(player->getVelocity()->x, 0);

            }

            // Create colliders iterator
            std::list<SDL_Rect>::iterator iterCol2;

            for (iterCol2 = player->getColliders()->begin(); iterCol2 != player->getColliders()->end(); ++iterCol2) {

                iterCol2->x = player->getPosition()->x;
                iterCol2->y = player->getPosition()->y;

                //std::cout << "COLLIDED coordinates: " << iterCol2->x << "," << iterCol2->y << "\n";
            }

            // set as grounded if 
            player->setIsGrounded(player->getAcceleration()->y > 0);
        }
    }

    bool PlayerController::getCanPress() {
        return canPressInput;
    }

    void PlayerController::setCanPress(bool canPress) {
        canPressInput = canPress;
    }

    /**
     * Checks for action inputs from keys
     */
    void PlayerController::actionInput(InputHandler *inputHandler) {
        setCanPress(false);
        if (inputHandler->keyboard[SDL_SCANCODE_E] == 2) {
            std::cout << "ACTION PRESSED\n";  // TESTING!!!
        }
        if (inputHandler->keyboard[SDL_SCANCODE_MINUS] == 2) {
            if (proportionalScalingActive) {
                proportionalScalingActive = false;
            } else {
                proportionalScalingActive = true;
            }
            
            std::cout << "SCALING BUTTON PRESSED\n";  // TESTING!!!
        }
    }

    void PlayerController::setPlayer(Entities::Player *p) {
        this->player = p;
    }
}
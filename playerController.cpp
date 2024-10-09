#include <map>

#include "playerController.h"

#include "global.h"
#include "SDL.h"
#include "collisions.h"
#include "definitions.h"
#include "physics.h"
#include "structs.h"
#include "input.h"

namespace Controllers {
    
    /**
     * Constructor for PlayerController
     * @param *player pointer to player to control
     */
    PlayerController::PlayerController(Entities::Player *player, EntityController *entityController, Timeline *timeline){
        this->player = player;
        this->entityController = entityController;
        this->m_timeline = timeline;
    }

    void PlayerController::updatePlayerPhysics(Physics *physics) {
         //physics->updateEntityPhysicsVectors(m_timeline, player);
        physics->applyGravity(player, m_timeline);
    }

    /**
     * Checks for movement inputs from keys and applies movement to player
     */
    void PlayerController::movementInput(InputHandler *inputHandler) {
        if (canPressInput) {
            // gets maxSpeed from player
            float speed = player->getMaxSpeed(); // this is technically being used more as a rate of acceleration right now
           
            Utils::Vector2D moveVector = Utils::Vector2D(0, 0);

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
                    // nothing
                }
                // Jump
                if (inputHandler->keyboard[SDL_SCANCODE_UP] == 1 && player->getIsGrounded()) {
                    moveVector.y = player->getJumpVector()->y;
                    player->setIsGrounded(false);
                    std::cout << "JUMPER: " << moveVector.y << "\n";
                }
                else {
                    // gravity
                    moveVector.y += player->getAcceleration()->y;
                }
            }
            // vertical movement (top down)
            else {
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
                moveVector.x /= 0.525322;
                moveVector.y /= 0.525322;
            }
           // moveVector.multConst(m_timeline->getDeltaTime() / MICROSEC_PER_SEC);

            //player->updateVelocity(moveVector);


            std::cout << "moveVector: " << moveVector.x << ", " << moveVector.y << "\n";

            // move on x axis
            movePlayer(Utils::Vector2D(moveVector.x, NULL), 0);
            // move on y axis
            movePlayer(Utils::Vector2D(NULL, moveVector.y), 1);
        }
    }

    /**
     * Moves the player by a given amount by manipulating its velocity vector
     * @param movementVector The vector that is changes the player's movement
     * @param axis where movement is occuring. 0 = x-axis, 1 = y-axis
     */
    void PlayerController::movePlayer(Utils::Vector2D movementVector, int axis) {
        Utils::Vector2D moveVector;

        float deltaTimeInSecs = static_cast<float>(m_timeline->getDeltaTime()) / MICROSEC_PER_SEC;
        Utils::Vector2D posMover = Utils::Vector2D(0, 0); // amount to change positon by
        Utils::Vector2D velMover = Utils::Vector2D(0, 0); // amount to change velocity by
        float decelerationRate = 10;

        // moving on x-axis
        if (axis == 0) {

            moveVector = Utils::Vector2D(movementVector.x * deltaTimeInSecs, 0);

            if (movementVector.x == 0) { // decelerate-x
                // moves velocity towards zero.
                // if the absolute value of the player's velocity is less than 2, it will then cancel out the velocity with no decceleration.
                if (player->getVelocity()->x > 2) {
                    velMover = Utils::Vector2D((-decelerationRate * Utils::Vector2D(player->getVelocity()->x, 0).multConst(deltaTimeInSecs).getMagnitude()), 0);
                }
                else if (player->getVelocity()->x < -2) {
                    velMover = Utils::Vector2D((decelerationRate * Utils::Vector2D(player->getVelocity()->x, 0).multConst(deltaTimeInSecs).getMagnitude()), 0);
                }
                else {
                    velMover = Utils::Vector2D(-1 * player->getVelocity()->x, 0);
                }

                //player->updateVelocity(Utils::Vector2D((*player->getVelocity()).multConst(-1).x, 0));

            }
            else { // accelerate-x
                // check if changing directions
                if ((movementVector.x > 0 && player->getVelocity()->x < 0) || (movementVector.x < 0 && player->getVelocity()->x > 0)) {
                    // increase rate of accelaration until moving in desired direction
                    velMover = moveVector.multConst(decelerationRate);
                }
                else {
                    velMover = moveVector;
                }
                
            }

            player->updateVelocity(velMover);
            posMover = Utils::Vector2D(player->getVelocity()->x, 0).multConst(deltaTimeInSecs);
            //player->setVelocity(moveVector.x, player->getVelocity()->y);

        }
        // moving on y-axis
        else if (axis == 1) {

            moveVector = Utils::Vector2D(0, movementVector.y * deltaTimeInSecs);

            if (movementVector.y == 0) { // decelerate-y
                // moves velocity towards zero.
                // if the absolute value of the player's velocity is less than 2, it will then cancel out the velocity with no decceleration.
                if (player->getVelocity()->y > 2) {
                    velMover = Utils::Vector2D(0, (-5 * Utils::Vector2D(0, player->getVelocity()->y).multConst(deltaTimeInSecs).getMagnitude()));
                }
                else if (player->getVelocity()->y < -2) {
                    velMover = Utils::Vector2D(0, (5 * Utils::Vector2D(0, player->getVelocity()->y).multConst(deltaTimeInSecs).getMagnitude()));
                }
                else {
                    velMover = Utils::Vector2D(0, -1 * player->getVelocity()->y);
                }

                //player->updateVelocity(Utils::Vector2D(0, (*player->getVelocity()).multConst(-1).y));

            }
            else { // accelerate-y
                velMover = moveVector;

            }

            player->updateVelocity(velMover);
            posMover = Utils::Vector2D(0, player->getVelocity()->y).multConst(deltaTimeInSecs);
            //player->setVelocity( player->getVelocity()->x, moveVector.y);

        }
        else {
            moveVector = Utils::Vector2D(0, 0);
        }

        // move player
        player->updatePosition(posMover);

        std::cout << "Vel: " << player->getVelocity()->x << ", " << player->getVelocity()->y << "\n";
        std::cout << "posMover: " << posMover.x << ", " << posMover.y << "\n";
        
        //player->updateVelocity(moveVector);
        //player->updatePosition(moveVector);

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
            if (axis == 0) { // x-axis collision
                //std::cout << "X-HIT\n";
                player->updatePosition(posMover.multConst(-1));
                player->updateVelocity(Utils::Vector2D(-1 * player->getVelocity()->x, 0));
            }
            else if (axis == 1) { // y-axis collision
                //std::cout << "Y-HIT\n";

                // set as grounded if player was moving down during y-axis collision
                player->setIsGrounded(player->getVelocity()->y >= 0);

                // update position and velocity of player
                player->updatePosition(posMover.multConst(-1));
                player->updateVelocity(Utils::Vector2D(0, -1 * player->getVelocity()->y));

            }
            
            // Create colliders iterator
            std::list<SDL_Rect>::iterator iterCol2;

            for (iterCol2 = player->getColliders()->begin(); iterCol2 != player->getColliders()->end(); ++iterCol2) {

                iterCol2->x = player->getPosition()->x;
                iterCol2->y = player->getPosition()->y;

                //std::cout << "COLLIDED coordinates: " << iterCol2->x << "," << iterCol2->y << "\n";
            }


        }
        // if there is no collision on the y-axis, set isGrounded to false
        else if (axis == 1) {
            player->setIsGrounded(false);
            
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
        // Action
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
        // Pauses and unpauses the Player Timeline
        if (inputHandler->keyboard[SDL_SCANCODE_P] == 2) {
            if (m_timeline->isPaused()) {
                std::cout << "Unpause\n";
                m_timeline->unpause();
				canPressInput = true;
            } else {
				std::cout << "Pause\n";
				m_timeline->pause();
                canPressInput = false;
            }
        }
        // Change timescale for player
        if (inputHandler->keyboard[SDL_SCANCODE_8] == 2) {
			std::cout << "Timescale 0.5\n";
            m_timeline->changeTimeScale(0.5f);
        }
		if (inputHandler->keyboard[SDL_SCANCODE_9] ==  2) {
			std::cout << "Timescale 1.0";
			m_timeline->changeTimeScale(1.0f);
		}
		if (inputHandler->keyboard[SDL_SCANCODE_0] == 2) {
			std::cout << "Timescale 2.0";
			m_timeline->changeTimeScale(2.0f);
		}
    }

    void PlayerController::setPlayer(Entities::Player *p) {
        this->player = p;
    }
}
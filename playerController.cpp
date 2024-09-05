#include "playerController.h"

#include "global.h"

namespace Controllers {
    
    /**
     * Constructor for PlayerController
     * @param *player pointer to player to control
     */
    PlayerController::PlayerController(Entities::Player *player){
        this->player = player;
    }

    /**
     * Checks for movement inputs from keys and applies movement to player
     */
    void PlayerController::movementInput() {
        // gets maxSpeed from player
        float speed = player->getMaxSpeed();
        Utils::Vector2D moveVector = Utils::Vector2D(0,0);
        moveVector.y = player->getAcceleration()->y;
        
        

        // horizontal movement
        if (inputHandler.keyboard[SDL_SCANCODE_RIGHT] == 1) {
            if (useAcceleration) {
                moveVector.x += speed;
            }
            else {
                moveVector.x = speed;
            }
        }
        if (inputHandler.keyboard[SDL_SCANCODE_LEFT] == 1) {
            if (useAcceleration) {
                moveVector.x -= speed;
            }
            else {
                moveVector.x = -speed;
            }
        }

        // vertical movement (platformer)
        if (platformerMovement) {
            if (inputHandler.keyboard[SDL_SCANCODE_DOWN] == 1) {

            }
            // Jump
            if (inputHandler.keyboard[SDL_SCANCODE_UP] == 1 && player->getIsGrounded()) {
                moveVector.y = player->getJumpVector()->y;
                player->setIsGrounded(false);
            }
        }
        else {
            // vertical movement (top down)
            if (inputHandler.keyboard[SDL_SCANCODE_DOWN] == 1) {
                if (useAcceleration) {
                    moveVector.y += speed;
                }
                else {
                    moveVector.y = speed;
                }
            }
            if (inputHandler.keyboard[SDL_SCANCODE_UP] == 1) {
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
        
        //std::cout << "movementMagnitude:" << moveVector.getMagnitude() << "\n";  // TESTING!!!
        //if (useAcceleration) {
        //    player->move(moveVector);
        //    //physics.applyForce(player, moveVector);
        //    //std::cout << "applyForce: (" << moveVector.x << ", " << moveVector.y << ")\n";
        //}
        //else {
        //    player->move(moveVector);
        //}
        player->setVelocity(moveVector.x, moveVector.y);
        player->move(moveVector);
        
    }

    /**
     * Checks for action inputs from keys
     */
    void PlayerController::actionInput() {
        if (inputHandler.keyboard[SDL_SCANCODE_E] == 1) {
            std::cout << "ACTION PRESSED\n";  // TESTING!!!
        }
    }

    void PlayerController::setPlayer(Entities::Player *p) {
        this->player = p;
    }
}
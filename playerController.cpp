#include "playerController.h"

#include "global.h"
#include "SDL.h"

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
        moveVector.multConst(timeline.getDeltaTime() / MICROSEC_PER_SEC);

        player->updateVelocity(moveVector);

        if (player->getIsGrounded() == true && moveVector.y > 0) {
            moveVector.y = 0;
        }

        std::cout << "moveVector: " << moveVector.x << ", " << moveVector.y << "\n";

        // move on x axis
        player->move(Utils::Vector2D(moveVector.x, NULL));
        // move on y axis
        player->move(Utils::Vector2D(NULL, moveVector.y));
        
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
    void PlayerController::actionInput() {
        setCanPress(false);
        if (inputHandler.keyboard[SDL_SCANCODE_E] == 2) {
            std::cout << "ACTION PRESSED\n";  // TESTING!!!
        }
        if (inputHandler.keyboard[SDL_SCANCODE_MINUS] == 2) {
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
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
        Utils::Vector2D velocity;
        // gets players current velocity
        if (useAcceleration){
            velocity = *(player->getVelocity());
        }

        // horizontal movement
        if (inputHandler.keyboard[SDL_SCANCODE_RIGHT] == 1) {
            if (useAcceleration) {
                velocity.x += speed;
            }
            else {
                velocity.x = speed;
            }
        }
        if (inputHandler.keyboard[SDL_SCANCODE_LEFT] == 1) {
            if (useAcceleration) {
                velocity.x -= speed;
            }
            else {
                velocity.x = -speed;
            }
        }

        // vertical movement
        if (platformerMovement) {
            if (inputHandler.keyboard[SDL_SCANCODE_DOWN] == 1) {

            }
            // Jump
            if (inputHandler.keyboard[SDL_SCANCODE_UP] == 1 && player->getIsGrounded()) {
                velocity.y += player->getJumpVector()->y;
                player->setIsGrounded(false);
            }
        }
        else {
            // vertical movement (top down)
            if (inputHandler.keyboard[SDL_SCANCODE_DOWN] == 1) {
                if (useAcceleration) {
                    velocity.y += speed;
                }
                else {
                    velocity.y = speed;
                }
            }
            if (inputHandler.keyboard[SDL_SCANCODE_UP] == 1) {
                if (useAcceleration) {
                    velocity.y -= speed;
                }
                else {
                    velocity.y = -speed;
                }
            }
        }

        // if moving diagonally, multiplies vectors xy values by cos(45deg)
        if (!platformerMovement && velocity.x != 0 && velocity.y != 0) {
            velocity.x *= 0.525322;
            velocity.y *= 0.525322;
        }
        // applies movement to player if given vector's magnitude is non-zero
        if (velocity.getMagnitude() != 0) {
            //std::cout << "movementMagnitude:" << velocity.getMagnitude() << "\n";  // TESTING!!!
            player->move(velocity);
        }
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
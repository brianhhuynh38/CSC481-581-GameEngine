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

        // horizontal movement
        if (inputHandler.keyboard[SDL_SCANCODE_RIGHT] == 1) {
            movementVector.x += speed;
        }
        if (inputHandler.keyboard[SDL_SCANCODE_LEFT] == 1) {
            movementVector.x -= speed;
        }
        // vertical movement
        if (inputHandler.keyboard[SDL_SCANCODE_DOWN] == 1) {
            movementVector.y += speed;
        }
        if (inputHandler.keyboard[SDL_SCANCODE_UP] == 1) {
            movementVector.y -= speed;
        }

        // if moving diagonally, multiplies vectors xy values by cos(45deg)
        if (movementVector.x != 0 && movementVector.y != 0) {
            movementVector.x *= 0.525322;
            movementVector.y *= 0.525322;
        }

        // applies movement to player if given vector's magnitude is non-zero
        if (movementVector.getMagnitude() != 0) {
            std::cout << "movementMagnitude:" << movementVector.getMagnitude() << "\n";  // TESTING!!!
            player->move(movementVector);
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
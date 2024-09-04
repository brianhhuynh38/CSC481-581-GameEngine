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
        if (inputHandler.keyboard[SDL_SCANCODE_RIGHT]) {
            movementVector.x += speed;
        }
        if (inputHandler.keyboard[SDL_SCANCODE_LEFT]) {
            movementVector.x -= speed;
        }
        if (inputHandler.keyboard[SDL_SCANCODE_DOWN]) {
            movementVector.y += speed;
        }
        if (inputHandler.keyboard[SDL_SCANCODE_UP]) {
            movementVector.y -= speed;
        }
        // if moving diagonally, multiplies vectors xy values by cos(45deg)
        if (movementVector.x != 0 && movementVector.y != 0) {
            movementVector.x *= 0.525322;
            movementVector.y *= 0.525322;
        }

        // Moves player if given vector's magnitude is not zero
        if (movementVector.getMagnitude() != 0) {
            player->move(movementVector);
        }
    }

    /**
     * Checks for action inputs from keys
     */
    void actionInput() {
        if (inputHandler.keyboard[SDL_SCANCODE_E]) {
            
        }
    }
}
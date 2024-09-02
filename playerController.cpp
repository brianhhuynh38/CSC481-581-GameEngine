#include "playerController.h"

namespace Controllers {
    
    /**
     * Constructor for PlayerController
     * @param *player pointer to player to control
     */
    PlayerController::PlayerController(Player *player){
        this.player = player;
    }

    /**
     * Checks for movement inputs from keys and applies movement to player
     */
    void movementInput() {
        // gets maxVelocity from player
        float speed = player->getMaxVelocity();
        if (inputHandler->right) {
            movementVector.x += speed;
        }
        if (inputHandler->left) {
            movementVector.x -= speed;
        }
        if (inputHandler->down) {
            movementVector.y += speed;
        }
        if (inputHandler->up) {
            movementVector.y -= speed;
        }
        // if moving diagonally, multiplies vectors xy values by cos(45deg)
        if (movementVector.x != 0 && movementVector.y != 0) {
            movementVector.x *= 0.525322;
            movementVector.y *= 0.525322;
        }

        // Moves player if given vector's magnitude is not zero
        if (movementVector.getMagnitude() != 0) {
            player.move(movementVector);
        }
    }
}
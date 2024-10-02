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
        moveVector.y = player->getVelocity()->y;

        // horizontal movement
        if (inputHandler.keyboard[SDL_SCANCODE_RIGHT] == 1) {
            moveVector.x += speed;
        }
        if (inputHandler.keyboard[SDL_SCANCODE_LEFT] == 1) {
            moveVector.x -= speed;
        }

        // vertical movement (platformer)
        if (platformerMovement) {
            if (inputHandler.keyboard[SDL_SCANCODE_DOWN] == 1) {

            }
            // Jump
            if (inputHandler.keyboard[SDL_SCANCODE_UP] == 1 && player->getIsGrounded()) {
                //physics.applyForce(player, player->getJumpVector()->multConst(1000));
                moveVector.y = player->getJumpVector()->y;
                player->setIsGrounded(false);
                std::cout << "JUMP!\n"; // testing
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

        moveVector = moveVector.multConst(timeline.getDeltaTime() / MICROSEC_PER_SEC);

        movePlayer(moveVector);


        
    }

    bool PlayerController::getCanPress() {
        return canPressInput;
    }

    void PlayerController::setCanPress(bool canPress) {
        canPressInput = canPress;
    }

    Uint32 my_callbackfunc(Uint32 interval, void* param) {
        return NULL;
    }

    /**
     * Checks for action inputs from keys
     */
    void PlayerController::actionInput() {
        setCanPress(false);
        if (inputHandler.keyboard[SDL_SCANCODE_E] == 1) {
            std::cout << "ACTION PRESSED\n";  // TESTING!!!
        }
        if (inputHandler.keyboard[SDL_SCANCODE_MINUS] == 1) {
            std::cout << "SCALING BUTTON PRESSED\n";  // TESTING!!!
        }
        SDL_AddTimer((33 / 10) * 10, my_callbackfunc, {this});
    }

    void PlayerController::setPlayer(Entities::Player *p) {
        this->player = p;
    }

    /**
     * Moves the player by a given amount by manipulating its velocity vector
     * @param movementVector The vector that is changes the player's movement
     */
    void PlayerController::movePlayer(Utils::Vector2D movementVector) {
        
        int moveMultiplier = 10; // temporary fix

        // quick fix for geting to fast 

        float maxMove = 64;
        if (movementVector.x > maxMove) {
            movementVector.x = maxMove;
        }
        else if (movementVector.x < -maxMove) {
            movementVector.x = -maxMove;
        }
        if (movementVector.y > maxMove) {
            movementVector.y = maxMove;
        }
        else if (movementVector.y < -maxMove) {
            movementVector.y = -maxMove;
        }


        // update acceleration/velocity of player
        if (movementVector.x != 0) { // moving on x-axis
            player->updateAcceleration(Utils::Vector2D(movementVector.x * moveMultiplier, 0));
            player->updateVelocity(Utils::Vector2D(movementVector.x * moveMultiplier, 0));
            //setVelocity(movementVector.x * moveMultiplier, getVelocity()->y);
            std::cout << "test " << movementVector.x << "\n";
            
        }
        else { // decelerate on x-axis if no horizontal movement input given
            player->updateAcceleration(Utils::Vector2D(player->getAcceleration()->x * -1, 0));
            player->updateVelocity(Utils::Vector2D(player->getVelocity()->x * -1, 0));
            //player->setAcceleration(0, NULL);
            //player->setVelocity(0, NULL);
        }

        if (movementVector.y != 0) { // moving on y-axis
            player->updateAcceleration(Utils::Vector2D(0, movementVector.y));

        }
    }
}
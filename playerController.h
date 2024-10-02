#include <SDL.h>
#include <iostream>
#include "vector2D.h"
#include "structs.h"
#include "player.h"

#pragma once
#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

namespace Controllers {
    class PlayerController {
    private:
        /** whether or not this controller is active.*/
        bool active = true;
        /** Pointer to player to control */
        Entities::Player* player;
        /** whether or not this controller uses acceleration in it's movement calculations */
        bool useAcceleration = false;
        /** sets movement to move like a platformer (temp value) */
        bool platformerMovement = true;

        bool canPressInput = true;

        /** 2D movement vector to be applied to the player */
        //Utils::Vector2D movementVector = Utils::Vector2D(0,0);
    public:
        /**
         * Constructor for PlayerController
         * @param *player pointer to player to control
         */
        PlayerController(Entities::Player* player);

        /**
         * Checks for movement inputs from keys and applies movement to player
         */
        void movementInput();

        /**
         * Checks for action inputs from keys
         */
        void actionInput();

        void setPlayer(Entities::Player *p);

        void movePlayer(Utils::Vector2D movementVector);

        bool getCanPress();

        void setCanPress(bool canPress);

        //void setMovementRestrictions();

        //void setPlayerState();

    };
}

#endif

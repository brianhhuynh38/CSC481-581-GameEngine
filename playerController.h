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

        /** 2D movement vector to be applied to the player */
        Utils::Vector2D movementVector;
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

        void setPlayer(Entities::Player newPlayer);

        //void setMovementRestrictions();

        //void setPlayerState();

    };
}

#endif

#include <SDL.h>
#include <iostream>
#include "vector2D.h"
#include "player.h"
#include "entityController.h"
#include "timeline.h"
#include "structs.h"

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
        /** Toggles whether player can input controls */
        bool canPressInput = true;
        /** Reference to entity controller for detecting collisions between the player and other entities */
        EntityController* entityController;
        /** Player timeline that controls player time */
        Timeline *m_timeline;

        /** 2D movement vector to be applied to the player */
        //Utils::Vector2D movementVector = Utils::Vector2D(0,0);
    public:
        /**
         * Constructor for PlayerController
         * @param player pointer to player to control
         * @param entityController: Pointer to EntityController to obtain list of entities
         */
        PlayerController(Entities::Player* player, EntityController* entityController, Timeline* timeline);

        /**
        * Updates the player physics and movement vector changes
        */
        void updatePlayerPhysics(Physics* physics);

        /**
         * Moves the player given a movement vector
         *
         * @param movementVector The vector used to move by adding to velocity
         */
        void movePlayer(Utils::Vector2D movementVector);

        /**
         * Checks for movement inputs from keys and applies movement to player
         */
        void movementInput(InputHandler* inputHandler);

        /**
         * Checks for action inputs from keys
         */
        void actionInput(InputHandler* inputHandler);

        void setPlayer(Entities::Player *p);

        bool getCanPress();

        void setCanPress(bool canPress);

        //void setMovementRestrictions();

        //void setPlayerState();

    };
}

#endif

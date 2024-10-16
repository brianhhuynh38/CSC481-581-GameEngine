#pragma once

#ifndef PLAYERGO_H
#define PLAYERGO_H

// - Player: TextureMesh, RigidBody (collision), PlayerInput
class PlayerGO : public GameObject {
public:
    PlayerGO(float scaleX, float scaleY, float positionX, float positionY, float width, float height, float mass,
        std::string textureFilepath, bool isStatic, bool isTrigger, float jumpVectorX, float jumpVectorY,
        float maxSpeed, Physics* physicsRef);
};

#endif

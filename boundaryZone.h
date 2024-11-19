#pragma once

#ifndef BOUNDARYZONE_H
#define BOUNDARYZONE_H

#include "GameObject.h"
#include "rigidBody.h"
#include "textureMesh.h"

class BoundaryZone : public GameObject {
protected:
	/** The reference to the global camera offset value used to keep track of the camera's position */
	Utils::Vector2D* m_cameraOffset;
	/* Initial camera position before collision */
	Utils::Vector2D m_pos1;
	/* Next camera position after collision */
	Utils::Vector2D m_pos2;
	/** Timer used to establish a cooldown between switching camera positions */
	int m_timer;
public:
	BoundaryZone(float scaleX, float scaleY, float positionX, float positionY, float width, float height, Utils::Vector2D* cameraPos, float mass,
		std::string textureFilepath, bool isKinematic, Utils::Vector2D pos1, Utils::Vector2D pos2);

	Utils::Vector2D getCurrentPos();

	void setCurrentPos(Utils::Vector2D newPos);

	Utils::Vector2D getPos1();

	Utils::Vector2D getPos2();

	void update(double deltaTimeInSecs, int64_t currentTime) override;

	void initiateTimer(int timeToSet);

	bool checkCooldown();
};

#endif

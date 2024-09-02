#include "entity.h"

#pragma once
#ifndef OSCILLATINGENTITY_H
#define OSCILLATINGENTITY_H

namespace Entities {

	class OscillatingEntity : Entity {
	private:
		OscillationType type;
		float amplitude;
		float frequency;
		int pauseTimer;
	public:
		OscillatingEntity(float scaleX, float scaleY, float positionX, float positionY, 
			float mass, float amplitude, float frequency, int pauseTimer,
			const char* textureFilepath, bool isStationary, bool affectedByPhysics);

		void oscillate();

		float getAmplitude();
		void setAmplitude(float amplitude);

		float getFrequency();
		void setFrequency(float frequency);
	};

	enum OscillationType {
		SIN,
		COS
	};
}

#endif
#pragma once
#ifndef ENTITY_H
#define ENTITY_H

namespace Entities {

	class Entity {
	private:
		float *m_scaleX;
		float* m_scaleY;
		SDL_Point *m_position;
		SDL_Point *m_velocity;
		SDL_Point *m_acceleration;
		SDL_Texture *m_texture;
		bool m_isStationary;
		bool m_isPlayer;
	public:

		Entity(float scaleX, float scaleY, SDL_Point position, char* textureFilepath, bool isStationary, bool isPlayer);

		SDL_Point *getPosition(void);

		SDL_Point *getVelocity(void);

		SDL_Point *getAcceleration(void);

		SDL_Texture* getTexture(void);

		void setTexture(char* textureFilepath);

		void destroy(void);
	};
}

#endif

#include <SDL.h>
#include <iostream>

#include "structs.h"

namespace GEngine::Entities {
	class Entity {
	private:
		SDL_Point *m_size;
		SDL_Point *m_position;
		SDL_Point *m_velocity;
		SDL_Point *m_acceleration;
		SDL_Texture *m_texture;
	public:
		Entity(SDL_Point size, SDL_Point position, SDL_Texture* texture) {
			*m_size = size;
			*m_position = position;
			*m_velocity = SDL_Point();
			*m_acceleration = SDL_Point();
			m_texture = texture;
		}

		SDL_Point *getPosition() {
			return m_position;
		}

		SDL_Point *getVelocity() {
			return m_velocity;
		}

		SDL_Point *getAcceleration() {
			return m_acceleration;
		}
	};
}

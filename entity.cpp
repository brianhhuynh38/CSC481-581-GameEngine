#include <SDL.h>

#include "structs.h"

namespace GEngine::Entities {
	class Entity {
	private:
		Vector m_size;
		Vector m_position;
		Vector m_velocity;
		SDL_Texture* m_texture;
	public:
		Vector getPosition() {
			return m_position;
		}

		Vector getVelocity() {
			return m_velocity;
		}

		void LoadEntity(char* filepath) {

		}
	};
}

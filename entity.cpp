#include <SDL.h>

#include "structs.h"

namespace GEngine::Entities {
	class Entity {
	public:
		Vector getPosition() {
			return position;
		}

		Vector getVelocity() {
			return velocity;
		}

		void LoadEntity(char* filepath) {

		}
	private:
		Vector size;
		Vector position;
		Vector velocity;
		SDL_Texture* texture;
	};
}

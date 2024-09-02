#pragma once
#ifndef DRAW_H
#define DRAW_H

namespace Render {

	void prepareScene(void);

	void presentScene(void);

	SDL_Texture* loadTexture(const char* filename);

	void displayTexture(SDL_Texture* texture, int x, int y);
}

#endif

#pragma once
#ifndef DRAW_H
#define DRAW_H

void prepareScene(SDL_Renderer* renderer);

void presentScene(SDL_Renderer* renderer);

SDL_Texture *loadTexture(char *filename);

void blit(SDL_Texture *texture, int x, int y);

#endif

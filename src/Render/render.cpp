#include "render.h"
#include "../globals.h"
SDL_Texture* cargarLaImagen(string filename, SDL_Renderer* renderer)
{
	SDL_Texture* texture; //variable temporal
	texture = IMG_LoadTexture(renderer, filename.c_str());
	return texture;
}

void mostrarLaImagen(SDL_Texture* texture, int x, int y, int w, int h, SDL_Renderer* renderer)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = w;
    dest.h = h;

    SDL_RenderCopy(renderer, texture, NULL, &dest);
}


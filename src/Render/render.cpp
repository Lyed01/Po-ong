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

SDL_Texture* GenerarTexto(SDL_Renderer* renderer, SDL_Surface* surface, SDL_Texture* texture, TTF_Font* fuente, SDL_Color color, string textoElegido)
{
    surface = TTF_RenderText_Blended(fuente, textoElegido.c_str(), color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    return texture;
}

void renderTexto(const string& texto, int x, int y, SDL_Color color, TTF_Font* fuente) {
    SDL_Surface* surface = TTF_RenderText_Solid(fuente, texto.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    int texW, texH;
    SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
    SDL_Rect rect = { x, y, texW, texH };
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void mostrarPuntajes(TTF_Font* fuente, int puntaje, bool esJugador) {
    string texto = to_string(puntaje);
    textoSurface = TTF_RenderText_Solid(fuente, texto.c_str(), {250,250,250});
    textoTexture = SDL_CreateTextureFromSurface(renderer, textoSurface);

    int texW = 0, texH = 0;

    SDL_QueryTexture(textoTexture, nullptr, nullptr, &texW, &texH);
    SDL_Rect dstRect = { 1200, 20, texW, texH };
    if (esJugador) {
        dstRect = { 40, 20, texW, texH };
    }
 
    SDL_RenderCopy(renderer, textoTexture, nullptr, &dstRect);
    SDL_FreeSurface(textoSurface);
    SDL_DestroyTexture(textoTexture);
}

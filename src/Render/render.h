//Declara funciones para cargar imágenes, dibujar texturas en pantalla, limpiar y actualizar el renderizador.

#pragma once
#include "render.h"
#include "../globals.h"
#include <SDL_ttf.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string>

using namespace std;


SDL_Texture* cargarLaImagen(string filename, SDL_Renderer* renderer);

void mostrarLaImagen(SDL_Texture* texture, int x, int y, int w, int h, SDL_Renderer* renderer);

void renderTexto(const string& texto, int x, int y, SDL_Color color, TTF_Font* fuente);

void mostrarPuntajes(TTF_Font* fuente, int puntaje, bool esJugador);

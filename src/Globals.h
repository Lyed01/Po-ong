#pragma once

#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL.h>
#include "Player/player.h"
#include "Ball/ball.h"

// Declaraciones (extern => existe en algún .cpp)
extern SDL_Renderer* renderer;
extern SDL_Window* window;
extern SDL_Texture* imagenFondoJuego;
extern SDL_Texture* imagenFondoMenu;
extern SDL_Texture* imagenJugador;
extern SDL_Texture* imagenIA;
extern SDL_Texture* imagenPelota;
extern bool isGameRunning;
extern Player player;
extern Player ia;
extern Ball ball;

#endif

//Implementa la captura de eventos SDL, actualización de estados de teclas presionadas y comunicación de esas acciones al juego.
#include "input.h"
#include <SDL.h>
#include "../globals.h"
#include <iostream>
using namespace std;
int posXOriginal;
bool volver = false;

void onKeyUp(int keyCode)
{
	switch (keyCode)
	{
	case SDLK_UP:
		player.up = false;
		break;
	case SDLK_DOWN:
		player.down = false;
		break;
	case SDLK_RIGHT:
		player.special = false;
		break;
	case SDLK_w:
		player.up = false;
		break;
	case SDLK_s:
		player.down = false;
		break;
	case SDLK_d:
		player.special = false;
		break;
	default:
		break;
	}
}


void onKeyDown(int keyCode)
{
	switch (keyCode)
	{
	case SDLK_UP:
		player.up = true;
		break;
	case SDLK_DOWN:
		player.down = true;
		break;
	case SDLK_RIGHT:
		player.special = true;
		break;
	case SDLK_w:
		player.up = true;
		break;
	case SDLK_s:
		player.down = true;
		break;
	case SDLK_d:
		player.special = true;
		break;
	case SDLK_ESCAPE:
		isGameRunning = false;
	default:
		break;
	}
}

void processInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			onKeyDown(event.key.keysym.sym);
			break;
		case SDL_KEYUP:
			onKeyUp(event.key.keysym.sym);
			break;
		default:
			break;
		}
	}
}

void movement(float deltaTime)
{
	if (player.up)
		player.posY -= player.speed * deltaTime;
	if (player.down)
		player.posY += player.speed * deltaTime;

	if (player.special) {
		posXOriginal = player.posX;
		player.posX += player.speed * deltaTime;
		volver = true;
		for (int i = 1; i <= 10; i++) {

			if (volver) {
				if (player.posX > posXOriginal) {
					player.posX -= player.speed * deltaTime;
					if (player.posX < posXOriginal)
						player.posX = posXOriginal;
				}
				else {
					volver = false;
				}
			}
		}
	}
	
	if (volver) {
		if (player.posX > posXOriginal) {
			player.posX -= player.speed * deltaTime;
			if (player.posX < posXOriginal)
				player.posX = posXOriginal;
		}
		else {
			volver = false;
		}
	}
	
	ball.posX += ball.velocidadX* deltaTime;
	ball.posY += ball.velocidadY * deltaTime;
	
	// La IA mueve solo en updateGame (donde sigue la pelota), no acá
}

//variables l\ocales para movimiento

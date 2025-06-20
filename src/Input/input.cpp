//Implementa la captura de eventos SDL, actualización de estados de teclas presionadas y comunicación de esas acciones al juego.
#include "input.h"
#include <SDL.h>
#include "../Game/game.cpp"


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

void movement()
{
	if (player.up == true)
	{
		player.posY -= player.speed / 100.0;
	}
	if (player.down)
	{
		player.posY += player.speed / 100.0;
	}

	if (player.special == true) {
		posXOriginal = player.posX; // Guardar la posición original antes de moverse
		player.posX += player.speed / 10.0;
		volver = true;
	}
	//Agregar un clock para completar el volver. Luego implementarlo para la ia tambien
	if (volver == true) {
		// Volver suavemente hacia la posición original
		if (player.posX != posXOriginal) {
			player.posX -= player.speed / 2.0;
			player.posX = posXOriginal;
			volver = false;
		}
		else
		{
			volver = false; // Ya está en su lugar
		}
	}

	if (ia.up == true)
	{
		ia.posY -= ia.speed / 100.0;
	}
	if (ia.down)
	{
		ia.posY += ia.speed / 100.0;
	}

	ball.posX += ball.velocidadX / 100;
	ball.posY += ball.velocidadY / 100;
}



//variables locales para movimiento


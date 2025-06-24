//Implementa la captura de eventos SDL, actualización de estados de teclas presionadas y comunicación de esas acciones al juego.
#include "input.h"
#include <SDL.h>
#include "../globals.h"
#include <iostream>
using namespace std;
bool enDash = false;
bool volviendoDash = false;
float posXOriginal = 0.0f;
float dashCooldown = 7.0f;      // Cooldown en segundos
float dashTimer = 0.0f;

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
	// Reducir timer cooldown si está activo
	if (dashTimer > 0.0f) {
		dashTimer -= deltaTime;
		if (dashTimer < 0.0f)
			dashTimer = 0.0f;
	}

	// Solo permite dash si cooldown terminó
	if (player.special && !enDash && !volviendoDash && dashTimer == 0.0f) {
		enDash = true;
		posXOriginal = player.posX;
		dashTimer = dashCooldown;   // Reiniciar cooldown
	}

	// Mover solo si no está en dash o volviendo
	if (!enDash && !volviendoDash) {
		if (player.up)
			player.posY -= player.speed * deltaTime;
		if (player.down)
			player.posY += player.speed * deltaTime;
	}

	// Limitar posición vertical
	if (player.posY < 0) player.posY = 0;
	if (player.posY + player.height > 720) player.posY = 720 - player.height;

	if (enDash) {
		player.posX += player.speed * 1.5 * deltaTime;  // Avanza rápido
		if (player.posX >= posXOriginal + 150) {
			enDash = false;
			volviendoDash = true;
		}
	}
	else if (volviendoDash) {
		player.posX -= player.speed * 0.5 * deltaTime;  // Vuelve rápido
		if (player.posX <= posXOriginal) {
			player.posX = posXOriginal;
			volviendoDash = false;
		}
	}

	// Movimiento pelota
	ball.posX += ball.velocidadX * deltaTime;
	ball.posY += ball.velocidadY * deltaTime;
}





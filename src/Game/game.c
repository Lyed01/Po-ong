#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Render/render.h"
#include "input/input.h"
#include "Ball/ball.h"
#include "Player/player.h"
#include "IA/ia.h"
using namespace std;

// Variables estáticas para uso interno al archivo
static SDL_Renderer* renderer = nullptr;
static SDL_Window* window = nullptr;
static SDL_Texture* imagenJugador = nullptr;
static SDL_Texture* imagenIA = nullptr;
static SDL_Texture* imagenPelota = nullptr;
static bool isGameRunning = true;
static Barra player;
static Barra ia;
static Ball ball;
static float timeActual = SDL_GetTicks();
static void UpdateGame()
{
	movimiento();
	checkColisionsWall();
	checkColisionsPaleta();
}

static void InitGame()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("Mi ventana", 100, 100, 1280, 720, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);

	imagenJugador = CargarLaImagen("assets/img/player.png", renderer);
	imagenIA = CargarLaImagen("assets/img/enemy.png", renderer);
	imagenPelota = CargarLaImagen("assets/img/ball.png", renderer);
}

static void runGame()
{
	InitGame();
	

	while (isGameRunning)
	{
		//float tiempoAnterior = tiempoActual;
		//tiempoActual = SDL_getTicks;
		//float deltaTime = (timepoActual-tiemoAnterior);
		processInput();
		UpdateGame();
		Render();
	}

	// Limpiar recursos
	SDL_DestroyTexture(imagenJugador);
	SDL_DestroyTexture(imagenIA);
	SDL_DestroyTexture(imagenPelota);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}



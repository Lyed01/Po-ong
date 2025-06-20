#include <iostream>
#include <SDL_ttf.h>
#include "../Player/player.h"
#include "../Render/render.h"
#include "../Input/input.h"
#include "../Ball/ball.h"
#include "../IA/ia.h"

using namespace std;

SDL_Renderer* renderer;
SDL_Window* window;
SDL_Texture* imagenJugador;
SDL_Texture* imagenIA;
SDL_Texture* imagenPelota;
bool isGameRunning = true;
Player player;
Player ia;
Ball ball;

//float tiempoActual = SDL_GetTicks();


void render() {
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderClear(renderer);

	mostrarLaImagen(imagenIA, 1180, ia.posY, renderer);
	mostrarLaImagen(imagenJugador, 0, player.posY, renderer);
	mostrarLaImagen(imagenPelota, ball.posX, ball.posY, renderer);

	SDL_RenderPresent(renderer);
}

static void updateGame(){
	movement();
	//checkCollisionsWall();
	//checkColisionsPlayer();
}

static void initGame(){
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("Mi ventana", 100, 100, 1280, 720, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);

	imagenJugador = cargarLaImagen("assets/img/player.png", renderer);
	imagenIA = cargarLaImagen("assets/img/enemy.png", renderer);
	imagenPelota = cargarLaImagen("assets/img/ball.png", renderer);
}

static void runGame(){
	initGame();
	

	while (isGameRunning){
		//float tiempoAnterior = tiempoActual;
		//tiempoActual = SDL_getTicks;
		//float deltaTime = (timepoActual-tiemoAnterior);
		processInput();
		updateGame();
		render();
	}


	// Limpiar recursos
	SDL_DestroyTexture(imagenJugador);
	SDL_DestroyTexture(imagenIA);
	SDL_DestroyTexture(imagenPelota);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}



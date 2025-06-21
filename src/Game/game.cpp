#include <iostream>
#include <SDL_ttf.h>
#include "../Render/render.h"
#include "../Input/input.h"
#include "../Ball/ball.h"
#include "../IA/ia.h"
#include "../Player/player.h"
#include "../globals.h"

using namespace std;

// Definiciones reales
SDL_Renderer* renderer = nullptr;
SDL_Window* window = nullptr;
SDL_Texture* imagenJugador = nullptr;
SDL_Texture* imagenIA = nullptr;
SDL_Texture* imagenPelota = nullptr;
bool isGameRunning = true;
Player player;
Player ia;
Ball ball;

void checkCollisions(int ventanaAlto) {
    if (ball.posY <= 0 || ball.posY + ball.height >= ventanaAlto) {
        ball.velocidadY *= -1;
    }
}

void ball_init(float x, float y) {
    ball.posX = x;
    ball.posY = y;
    ball.velocidadX = 300;
    ball.velocidadY = 300;
    ball.width = 30;
    ball.height = 30;
}

void player_init(float x, float y) {
    player.posX = x;
    player.posY = y;
    player.width = 10;
    player.height = 60;
    player.speed = 300;
}

void ia_init(float x, float y) {
    ia.posX = x;
    ia.posY = y;
    ia.width = 10;
    ia.height = 60;
    ia.speed = 300;
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderClear(renderer);

    mostrarLaImagen(imagenIA, ia.posX, ia.posY, renderer);
    mostrarLaImagen(imagenJugador, player.posX, player.posY, renderer);
    mostrarLaImagen(imagenPelota, ball.posX, ball.posY, renderer);

    SDL_RenderPresent(renderer);
}

static void updateGame(float deltaTime) {
    movement(deltaTime);
    ball.posX += ball.velocidadX * deltaTime;
    ball.posY += ball.velocidadY * deltaTime;
    checkCollisions(720);


    // Colisión con paleta jugador
    if (ball.posX <= player.posX + player.width &&
        ball.posX + ball.width >= player.posX &&
        ball.posY + ball.height >= player.posY &&
        ball.posY <= player.posY + player.height) {
        ball.velocidadX *= -1;
    }

    // Colisión con paleta IA
    if (ball.posX + ball.width >= ia.posX &&
        ball.posX <= ia.posX + ia.width &&
        ball.posY + ball.height >= ia.posY &&
        ball.posY <= ia.posY + ia.height) {
        ball.velocidadX *= -1;
    }
}

static void initGame() {
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("Mi ventana", 100, 100, 1280, 720, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);

    imagenJugador = cargarLaImagen("assets/img/player.png", renderer);
    imagenIA = cargarLaImagen("assets/img/enemy.png", renderer);
    imagenPelota = cargarLaImagen("assets/img/ball.png", renderer);

    player_init(0, 300);
    ia_init(1180, 300);
    ball_init(640, 360);
}

void runGame() {
    initGame();
    Uint32 lastTick = SDL_GetTicks();

    while (isGameRunning) {
        Uint32 currentTick = SDL_GetTicks();
        float deltaTime = (currentTick - lastTick) / 1000.0f;
        lastTick = currentTick;

        processInput();
        updateGame(deltaTime);
        render();
    }

    SDL_DestroyTexture(imagenJugador);
    SDL_DestroyTexture(imagenIA);
    SDL_DestroyTexture(imagenPelota);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
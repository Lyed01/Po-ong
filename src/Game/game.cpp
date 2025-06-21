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
SDL_Texture* imagenFondoJuego = nullptr;
SDL_Texture* imagenFondoMenu = nullptr;
SDL_Texture* imagenJugador = nullptr;
SDL_Texture* imagenIA = nullptr;
SDL_Texture* imagenPelota = nullptr;
bool isGameRunning = true;
Player player;
Player ia;
Ball ball;

void checkCollisions(int ventanaAlto, int ventanaAncho) {
    if (ball.posY <= 0 || ball.posY + ball.height >= ventanaAlto) {
        ball.velocidadY *= -1;
    }
    if (ball.posX <= 0 || ball.posX + ball.width >= ventanaAncho) {
        ball.velocidadX *= -1;
    }
   
}

void ball_init(float x, float y) {
    ball.posX = x;
    ball.posY = y;
    ball.velocidadX = 1000;
    ball.velocidadY = 1000;
    ball.width = 10;
    ball.height = 10;
}

void player_init(float x, float y) {
    player.posX = x;
    player.posY = y;
    player.width = 10;
    player.height = 60;
    player.speed = 1000;
}

void ia_init(float x, float y) {
    ia.posX = x;
    ia.posY = y;
    ia.width = 10;
    ia.height = 60;
    ia.speed = 1000;
}

void render() {
    SDL_SetRenderDrawColor(renderer, 120, 140, 220, 200);
    SDL_RenderClear(renderer);
   
    mostrarLaImagen(imagenFondoJuego, NULL, NULL, renderer);
    mostrarLaImagen(imagenIA, ia.posX, ia.posY, renderer);
    mostrarLaImagen(imagenJugador, player.posX, player.posY, renderer);
    mostrarLaImagen(imagenPelota, ball.posX, ball.posY, renderer);

    SDL_RenderPresent(renderer);
}

static void updateGame(float deltaTime) {
    movement(deltaTime);
  
    checkCollisions(720, 1280);

    if (ball.posY < ia.posY) {
        ia.posY -= ia.speed / 100.0f;
    }
    if (ball.posY > ia.posY + ia.height) {
        ia.posY += ia.speed / 100.0f;
    }


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

    window = SDL_CreateWindow("Pong", 50, 50, 1280, 720, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);

    imagenFondoJuego = cargarLaImagen("assets/img/gameplay.png", renderer);
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
    SDL_DestroyTexture(imagenFondoJuego);
    SDL_DestroyTexture(imagenFondoMenu);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
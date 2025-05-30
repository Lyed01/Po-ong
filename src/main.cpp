#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Render/render_utils.h"
#include "input/input.h"

using namespace std;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("Pong", 100, 100, 1280, 720, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Texture* playerTexture = CargarLaImagen("assets/img/player.png", renderer);
    SDL_Texture* enemyTexture = CargarLaImagen("assets/img/enemy.png", renderer);

    bool isRunning = true;
    SDL_Event event;
    int previousTime = SDL_GetTicks();

    while (isRunning) {
        // Eventos
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                isRunning = false;
        }

        // Tiempo
        int currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - previousTime) / 1000.0f;
        previousTime = currentTime;

        // Render
        SDL_RenderClear(renderer);
        MostrarLaImagen(enemyTexture, 100, 500, false, 0, 0, renderer);
        MostrarLaImagen(playerTexture, 300, 500, true, 100, 100, renderer);
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // ~60 FPS
    }

    // Cleanup
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(enemyTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();

    return 0;
}

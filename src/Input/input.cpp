#include "input.h"
#include "../globals.h"
#include <SDL.h>

void onKeyUp(int keyCode) {
    switch (keyCode) {
    case SDLK_UP:
    case SDLK_w:
        player.up = false;
        break;
    case SDLK_DOWN:
    case SDLK_s:
        player.down = false;
        break;
    case SDLK_RIGHT:
    case SDLK_d:
        player.special = false;
        break;
    }
}

void onKeyDown(int keyCode) {
    switch (keyCode) {
    case SDLK_UP:
    case SDLK_w:
        player.up = true;
        break;
    case SDLK_DOWN:
    case SDLK_s:
        player.down = true;
        break;
    case SDLK_RIGHT:
    case SDLK_d:
        player.special = true;
        break;
    case SDLK_ESCAPE:
        isGameRunning = false;
        break;
    }
}

void processInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
            onKeyDown(event.key.keysym.sym);
            break;
        case SDL_KEYUP:
            onKeyUp(event.key.keysym.sym);
            break;
        }
    }
}
void movement(float deltaTime) {
    player.move(deltaTime, 720);
    ball.move(deltaTime);
}


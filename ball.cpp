#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "ball.h"

void checkCollisions(Ball* ball, int ventanaAncho, int ventanaAlto){
    if (ball.posY > ventanaAlto)
    {
        ball.velocidadY *= -1;
    }
   
    if (ball.posY < 0)
    {
        ball.velocidadY *= -1;
    }
}

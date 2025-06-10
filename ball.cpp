#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "pelota.h"

void CheckCollisions(Pelota* pelota, int ventanaAncho, int ventanaAlto)
{
    if (pelota.posY > ventanaAlto)
    {
        pelota.velocidadY *= -1;
    }

    if (pelota.posY < 0)
    {
        pelota.velocidadY *= -1;
    }

}

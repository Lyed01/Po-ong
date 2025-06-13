#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "ball.h"

void checkCollisionsWall(Ball* ball, int ventanaAncho, int ventanaAlto){
    if ((ball.posY > ventanaAlto) || (ball.posY < 0 ))
    {
        ball.velocidadY *= -1;
    }
    if ((bal.posX > VentanaAncho) || (ball.posX < 0)){
        //Funcion para sumar punto y reiniciar el tablero
    }
}

void checkCollisionsPlayer(Ball* ball, Player* player, Player* ia){
    if (ball.posX > (player.posX && player.w){
        break
    }
    if (ball.posY > (player.posY && player.h){
        break
    }
    if (ball.posX > (player.posX && player.w){
        break
    }
    if (ball.posX > (player.posX && player.w){
        break
    }
    else{
        ball.velocidadX *=-1;
    }
    
}

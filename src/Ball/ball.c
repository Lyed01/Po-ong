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

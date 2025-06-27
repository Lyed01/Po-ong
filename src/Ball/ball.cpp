#include "ball.h"
#include <cstdlib> 

void Ball::init(float x, float y, float vX, float vY) {
    posX = x;
    posY = y;
    velocidadX = vX;
    velocidadY = vY;
    width = 75;
    height = 75;
    habilidad = false;
    tiempoHabilidad = 0;

}

void Ball::move(float deltaTime) {
    posX += velocidadX * deltaTime;
    posY += velocidadY * deltaTime;
}

void Ball::aplicarHabilidad(float deltaTime) {
    habilidad = true;
    width = 150;
    height = 150;
    
}
void Ball::ballTimerHability(float deltaTime) {
    tiempoHabilidad += deltaTime;
    if (tiempoHabilidad >= 5) {
        width = 75;
        height = 75;
        habilidad = false;
        tiempoHabilidad = 0;
    }
}
void Ball::reset(int ventanaAncho, int ventanaAlto, bool haciaJugador) {
    posX = ventanaAncho / 2.0f;
    posY = ventanaAlto / 2.0f;

    velocidadX = haciaJugador ? 400.0f : -400.0f;
    velocidadY = (rand() % 2 == 0) ? 200.0f : -200.0f;
}

void Ball::limitarPantalla(int ventanaAlto) {
    if (posY <= 0) {
        posY = 0;
        velocidadY *= -1;
    }

    if (posY + height >= ventanaAlto) {
        posY = ventanaAlto - height;
        velocidadY *= -1;
    }

}

#pragma once

struct Ball {
    float posX, posY;
    float velocidadX, velocidadY;
    int width, height;
    bool habilidad;
    float tiempoHabilidad;

    void init(float x, float y, float vX = 500.0f, float vY = 300.0f);
    void move(float deltaTime);
    void aplicarHabilidad(float deltaTime);
    void reset(int ventanaAncho, int ventanaAlto, bool haciaJugador);
    void limitarPantalla(int ventanaAlto);
    void ballTimerHability(float deltatime);
};

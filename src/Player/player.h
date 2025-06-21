
// Declara las funciones para inicializar la paleta
// , actualizar su posición y dibujarla, además de funciones para obtener su posición o estado.
// player.h
#pragma once     

// Estructura del jugador
typedef struct Player {
    int posY;
    int posX;
    int width;
    int height;
    float speed;
    bool up = false;
    bool down = false;
    bool special = false;
    

} Player;


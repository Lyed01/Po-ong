
// Declara las funciones para inicializar la paleta
// , actualizar su posición y dibujarla, además de funciones para obtener su posición o estado.
// player.h
#ifndef PLAYER_H     
#define PLAYER_H      

// Estructura del jugador
typedef struct Player {
    int posY;
    int posX;
    float velocidad;
    bool arriba = false;
    bool abajo = false;
    bool special = false;
} Player;




#endif 

//Implementa la lógica para mover la paleta
// 
// según el input, manejar límites de movimiento y dibujar la paleta en pantalla.
// player.c
#include "player.h"   // ?? Incluye el header que define la estructura y las funciones
#include <stdio.h>    // ?? Para simular el dibujo usando printf (en un juego real usarías SDL o similar)

void player_init(Player* player, float x, float y) {
    player->x = x;         // ?? Establece la posición inicial en x
    player->y = y;         // ?? Establece la posición inicial en y
    player->width = 10;    // ?? Valor fijo de ancho
    player->height = 60;   // ?? Valor fijo de alto
    player->speed = 300;   // ?? El jugador se moverá 300 píxeles por segundo
}

void player_move_up(Player* player, float deltaTime) {
    player->y -= player->speed * deltaTime;  // ?? Mueve hacia arriba (y disminuye)
}

void player_move_down(Player* player, float deltaTime) {
    player->y += player->speed * deltaTime;  // ?? Mueve hacia abajo (y aumenta)
}

void player_draw(Player* player) {
    // ?? Simulamos el renderizado con un print (en un juego real usarías gráficos)
    printf("Player at (%.2f, %.2f)\n", player->x, player->y);
}


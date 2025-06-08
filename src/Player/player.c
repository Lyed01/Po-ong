//Implementa la lógica para mover la paleta según el input, manejar límites de movimiento y dibujar la paleta en pantalla.
#include "player.h"   // ?? Incluye el header que define la estructura y las funciones
#include <stdio.h>    

void player_init(Player* player, float x, float y) {
    player.x = x;         // ?? Establece la posición inicial en x
    player.y = y;         // ?? Establece la posición inicial en y
    player.width = 10;    // ?? Valor fijo de ancho
    player.height = 60;   // ?? Valor fijo de alto
    player.speed = 300;   // ?? El jugador se moverá 300 píxeles por segundo
}




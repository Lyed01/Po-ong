
// Declara las funciones para inicializar la paleta
// , actualizar su posición y dibujarla, además de funciones para obtener su posición o estado.
// player.h
#ifndef PLAYER_H      // ?? Si no está definido PLAYER_H...
#define PLAYER_H      // ?? ...defínelo ahora (protección contra inclusiones múltiples)

// Estructura del jugador
typedef struct {
    float posY;
    float posX;
    float velocidad;
    bool arriba;
    bool abajo;
    bool special = false;
} Player;

// Inicializa un jugador
//void player_init(Player* player, float x, float y);

// Mueve al jugador hacia arriba
//void player_move_up(Player* player, float deltaTime);

// Mueve al jugador hacia abajo
//void player_move_down(Player* player, float deltaTime);

// Dibuja al jugador (esto lo puedes conectar con tu sistema de renderizado)
//void player_draw(Player* player);

#endif // ?? Final del include guard

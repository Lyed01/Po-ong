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

#endif 

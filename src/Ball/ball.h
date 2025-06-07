#ifndef PELOTA_H
#define PELOTA_H

typedef struct Pelota
{
    float posX;
    float posY;
    float velocidadX;
    float velocidadY;
} Pelota;

// Revisa colisiones con los bordes de la ventana y ajusta la velocidad de la pelota
void CheckCollisions(Pelota* pelota, int ventanaAncho, int ventanaAlto);

#endif // PELOTA_H

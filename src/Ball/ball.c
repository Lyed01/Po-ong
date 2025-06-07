#include "pelota.h"

void CheckCollisions(Pelota* pelota, int ventanaAncho, int ventanaAlto)
{
    if (pelota->posY > ventanaAlto)
    {
        pelota->velocidadY *= -1;
    }
   
    if (pelota->posY < 0)
    {
        pelota->velocidadY *= -1;
    }
    
}

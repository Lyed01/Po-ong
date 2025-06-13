//Declara funciones para cargar imágenes, dibujar texturas en pantalla, limpiar y actualizar el renderizador.

#ifndef IMAGENES_H
#define IMAGENES_H

#include <string>
#include <SDL.h>
#include <SDL_image.h>

// Carga una imagen y devuelve la textura SDL correspondiente
SDL_Texture* cargarLaImagen(string filename, SDL_Renderer* renderer);

// Muestra la textura en las coordenadas (x, y)
void mostrarLaImagen(SDL_Texture* texture, int x, int y, SDL_Renderer* renderer);

#endif // IMAGENES_H

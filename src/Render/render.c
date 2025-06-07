//Implementa las funciones que manejan la carga de imáge	nes (texturas SDL), dibujo de sprites y manejo del buffer gráfico para mostrar en pantalla.

SDL_Texture* CargarLaImagen(string filename, SDL_Renderer* renderer)
{
	SDL_Texture* texture; //variable temporal
	texture = IMG_LoadTexture(renderer, filename.c_str());
	return texture;
}

void MostrarLaImagen(SDL_Texture* texture, int x, int y, SDL_Renderer* renderer)
{
	SDL_Rect dest; //vacio
  
  //Posicion
	dest.x = x;
	dest.y = y;

	//dimensiones
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	SDL_RenderCopy(renderer, texture, NULL, &dest);
}


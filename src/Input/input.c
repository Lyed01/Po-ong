//Implementa la captura de eventos SDL, actualización de estados de teclas presionadas y comunicación de esas acciones al juego.
void onKeyUp(int keyCode){
	switch (keyCode)
	{
	case SDLK_UP:
		IA.arriba = false;
		break;
	case SDLK_DOWN:
		IA.abajo = false;
		break;
	case SDLK_w:
		jugador.arriba = false;
		break;
	case SDLK_s:
		jugador.abajo = false;
		break;
	case SDLK_RIGHT:
		jugador.special = false;
		break;
	default:
		break;
	}
}

void onKeyDown(int keyCode){
	switch (keyCode)
	{
	case SDLK_UP:
		IA.arriba = true;
		break;
	case SDLK_DOWN:
		IA.abajo = true;
		break;
	case SDLK_w:
		jugador.arriba = true;
		break;
	case SDLK_s:
		jugador.abajo = true;
		break;
	case SDLK_RIGHT:
		jugador.special = true;
		break;
	case SDLK_ESCAPE:
		isGameRunning = false;
	default:
		break;
	}
}

void processInput(){
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		if SDL_KEYDOWN:
			onKeyDown(event.key.keysym.sym);
			break;
		case SDL_KEYUP:
			onKeyUp(event.key.keysym.sym);
			break;
		default:
			break;
		}
	}
}

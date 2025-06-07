//Declara funciones para inicializar y manejar la entrada del usuario (teclado, eventos SDL).

void Movement(){
	if (jugador.arriba == true)
	{
		jugador.posY -= jugador.velocidad / 100.0;
	}
	if (jugador.abajo)
	{
		jugador.posY += jugador.velocidad / 100.0;
	}

	if (jugador.special == true){
		posXOriginal = jugador.posX; // Guardar la posición original antes de moverse
		jugador.posX += jugador.velocidad / 10.0;
		volver = true;
	}
	if (volver == true) {
		// Volver suavemente hacia la posición original
		if (jugador.posX != posXOriginal){
			jugador.posX -= jugador.velocidad / 2.0;
			jugador.posX = posXOriginal;
			volver = false;
		}
		else
		{
			volver = false; // Ya está en su lugar
		}
	}

	if (IA.arriba == true)
	{
		IA.posY -= IA.velocidad / 100.0;
	}
	if (IA.abajo)
	{
		IA.posY += IA.velocidad / 100.0;
	}

	ball.posX += ball.velocidadX / 100;
	ball.posY += ball.velocidadY / 100;
}

//Declara funciones para inicializar y manejar la entrada del usuario (teclado, eventos SDL).

	case SDLK_RIGHT:
		jugador.special = true;
		break;

if (jugador.special == true)
{
	posXOriginal = jugador.posX; // Guardar la posición original antes de moverse
	jugador.posX += jugador.velocidad / 10.0;
	volver = true;
}
if (volver == true) {
		// Volver suavemente hacia la posición original
		if (jugador.posX > posXOriginal)
		{
			jugador.posX -= jugador.velocidad / 2.0;

			// Evitar pasarse del punto original
			if (jugador.posX <= posXOriginal)
			{
				jugador.posX = posXOriginal;
				volver = false;
			}
		}
		else
		{
			volver = false; // Ya está en su lugar
		}
	}

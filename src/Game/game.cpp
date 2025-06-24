#include <iostream>
#include <SDL_ttf.h>
#include "../Render/render.h"
#include "../Input/input.h"
#include "../Ball/ball.h"
#include "../IA/ia.h"
#include "../Player/player.h"
#include "../globals.h"

using namespace std;

// Definiciones reales
SDL_Renderer* renderer = nullptr;
SDL_Window* window = nullptr;
SDL_Texture* imagenFondoJuego = nullptr;
SDL_Texture* imagenFondoMenu = nullptr;
SDL_Texture* imagenJugador = nullptr;
SDL_Texture* imagenIA = nullptr;
SDL_Texture* imagenPelota = nullptr;
bool isGameRunning = true;
Player player;
Player ia;
Ball ball;
int puntosJugador = 0; 
int puntosIA = 0;
bool iaModoFullTracking = false;
float iaModeTimer = 0.0f;
bool enPausa = false;
float timerPausa = 0.0f;
const float duracionPausa = 2.0f; // segundos de pausa tras un puntobool iaEnDash = false;

bool iaEnDash = false;
bool iaVolviendoDash = false;
float iaPosXOriginal = 0.0f;
float iaDashCooldown = 7.0f;
float iaDashTimer = 0.0f;







void checkCollisions(int ventanaAlto, int ventanaAncho) {
    if (ball.posY <= 0) {
        ball.posY = 0;  // Reposicioná dentro del área
        ball.velocidadY *= -1;
    }

    if (ball.posY + ball.height >= ventanaAlto) {
        ball.posY = ventanaAlto - ball.height;  // Reposicioná dentro del área
        ball.velocidadY *= -1;
    }

   
   
   
}//Colisiones entre la pelota y la pantalla

void ball_init(float x, float y) {
    ball.posX = x;
    ball.posY = y;
    ball.velocidadX = 500;
    ball.velocidadY = 300;
    ball.width = 50;
    ball.height = 50;
}//inicializar objeto y caracteristicas

void player_init(float x, float y) {
    player.posX = x;
    player.posY = y;
    player.width = 50;
    player.height = 100;
    player.speed = 1000;
}//inicializar objeto y caracteristicas

void ia_init(float x, float y) {
    ia.posX = x;
    ia.posY = y;
    ia.width = 50;
    ia.height = 100;
    ia.speed = 800;
}//inicializar objeto y caracteristicas

void resetPositionsAndPause(int ventanaAncho, int ventanaAlto, bool puntoJugador) {
    // Reset pelota
    ball.posX = ventanaAncho / 2.0f;
    ball.posY = ventanaAlto / 2.0f;

    float velocidadInicialX = 400.0f;
    float velocidadInicialY = 200.0;

    ball.velocidadX = (puntoJugador) ? velocidadInicialX : -velocidadInicialX;
    ball.velocidadY = (rand() % 2 == 0) ? velocidadInicialY : -velocidadInicialY;

    // Reset jugadores (centrados verticalmente)
    player.posY = (ventanaAlto - player.height) / 2.0f;
    ia.posY = (ventanaAlto - ia.height) / 2.0f;

    // Entrar en pausa
    enPausa = true;
    timerPausa = duracionPausa;
}
void updateIA(float deltaTime, int canchaMitadX) {
    // === Control del cambio de modo (full tracking o medio campo) ===
    iaModeTimer += deltaTime;
    if (iaModeTimer >= 7.0f) {
        iaModoFullTracking = (rand() % 2 == 0);
        iaModeTimer = 0.0f;
    }

    // === Control de cooldown de dash ===
    if (iaDashTimer > 0.0f) {
        iaDashTimer -= deltaTime;
        if (iaDashTimer < 0.0f)
            iaDashTimer = 0.0f;
    }

    // === Ver si puede iniciar dash ===
    bool puedeDash = (iaDashTimer == 0.0f) && !iaEnDash && !iaVolviendoDash;
    bool pelotaCercaX = (ball.posX > ia.posX - 100) && (ball.posX < ia.posX + ia.width + 50);
    bool pelotaEnRangoY = (ball.posY + ball.height > ia.posY) && (ball.posY < ia.posY + ia.height);

    if (puedeDash && pelotaCercaX && pelotaEnRangoY) {
        iaEnDash = true;
        iaPosXOriginal = ia.posX;
        iaDashTimer = iaDashCooldown;
    }

    // === Movimiento horizontal (dash) ===
    if (iaEnDash) {
        ia.posX -= ia.speed * 1.5f * deltaTime;
        if (ia.posX <= iaPosXOriginal - 100) {
            iaEnDash = false;
            iaVolviendoDash = true;
        }
    }
    else if (iaVolviendoDash) {
        ia.posX += ia.speed * 0.5f * deltaTime;
        if (ia.posX >= iaPosXOriginal) {
            ia.posX = iaPosXOriginal;
            iaVolviendoDash = false;
        }
    }

    // === Movimiento vertical (tracking) ===
    bool seguirPelota = false;
    if (iaModoFullTracking) {
        seguirPelota = true;
    }
    else {
        if (ball.posX >= canchaMitadX) {
            seguirPelota = true;
        }
    }

    if (seguirPelota) {
        float iaCentroY = ia.posY + ia.height / 2.0f;
        float ballCentroY = ball.posY + ball.height / 2.0f;
        const float deadZone = 10.0f;

        if (ballCentroY < iaCentroY - deadZone) {
            ia.posY -= ia.speed * deltaTime;
        }
        else if (ballCentroY > iaCentroY + deadZone) {
            ia.posY += ia.speed * deltaTime;
        }
    }

    // === Oscilación leve ===
    static float oscilacionTimer = 0.0f;
    oscilacionTimer += deltaTime;
    float oscilacion = sin(oscilacionTimer * 2.0f) * 9.0f;
    ia.posY += oscilacion * deltaTime;

    // === Limitar dentro de la pantalla ===
    if (ia.posY < 0) ia.posY = 0;
    if (ia.posY + ia.height > 720) ia.posY = 720 - ia.height;
}



void resetGame(int ventanaAncho, int ventanaAlto, bool puntoJugador) {
    ball.posX = ventanaAncho / 2.0f;
    ball.posY = ventanaAlto / 2.0f;

    // Velocidad inicial
    float velocidadInicialX = 800.0f;
    float velocidadInicialY = 600.0f;

    // La pelota sale en dirección contraria al jugador que falló
    if (puntoJugador) {
        ball.velocidadX = velocidadInicialX;  // Sale hacia IA
    }
    else {
        ball.velocidadX = -velocidadInicialX; // Sale hacia jugador
    }

  

} //Resetea el juego

void checkPoints(int ventanaAncho, int ventanaAlto) {
    if (ball.posX + ball.width < 0) {
        puntosIA++;
        resetPositionsAndPause(ventanaAncho, ventanaAlto, false);
    }
    if (ball.posX > ventanaAncho) {
        puntosJugador++;
        resetPositionsAndPause(ventanaAncho, ventanaAlto, true);
    }
}


void render() {
    SDL_SetRenderDrawColor(renderer, 120, 140, 220, 200);
    SDL_RenderClear(renderer);
    mostrarLaImagen(imagenFondoJuego, 0, 0, 1280, 720, renderer);
    mostrarLaImagen(imagenPelota, (int)ball.posX, (int)ball.posY, ball.width, ball.height, renderer);
    mostrarLaImagen(imagenJugador, (int)player.posX, (int)player.posY, player.width, player.height, renderer);
    mostrarLaImagen(imagenIA, (int)ia.posX, (int)ia.posY, ia.width, ia.height, renderer);
    SDL_RenderPresent(renderer);
}//Muestra los objetos

static void updateGame(float deltaTime) {
    const float VELOCIDAD_INCREMENTO = 1.0008f;
    static float iaReactionTimer = 0.0f;

    if (enPausa) {
        timerPausa -= deltaTime;
        if (timerPausa <= 0.0f) {
            enPausa = false;
        }
        return; // Salimos sin actualizar movimientos ni colisiones
    }

    movement(deltaTime);
    checkPoints(1280, 720);
    checkCollisions(720, 1280);

    updateIA(deltaTime, 640);

    // === Colisión con paleta jugador ===
    if (ball.posX <= player.posX + player.width &&
        ball.posX + ball.width >= player.posX &&
        ball.posY + ball.height >= player.posY &&
        ball.posY <= player.posY + player.height) {

        ball.velocidadX = -ball.velocidadX * VELOCIDAD_INCREMENTO;

        // Corregir posición para que la pelota quede justo a la derecha de la paleta
        ball.posX = player.posX + player.width;

        // Ajustar velocidad vertical según impacto (opcional)
        float paletaCentroY = player.posY + player.height / 2.0f;
        float ballCentroY = ball.posY + ball.height / 2.0f;
        float diffY = ballCentroY - paletaCentroY;
        ball.velocidadY = diffY * 7;
    }

    // === Colisión con paleta IA ===
    if (ball.posX + ball.width >= ia.posX &&
        ball.posX <= ia.posX + ia.width &&
        ball.posY + ball.height >= ia.posY &&
        ball.posY <= ia.posY + ia.height) {

        ball.velocidadX = -ball.velocidadX * VELOCIDAD_INCREMENTO;

        // Corregir posición para que la pelota quede justo a la izquierda de la paleta IA
        ball.posX = ia.posX - ball.width;

        float paletaCentroY = ia.posY + ia.height / 2.0f;
        float ballCentroY = ball.posY + ball.height / 2.0f;
        float diffY = ballCentroY - paletaCentroY;
        ball.velocidadY = diffY * 7;
    }
}

static void initGame() {
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("Pong", 50, 50, 1280, 720, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);

    imagenFondoJuego = cargarLaImagen("assets/img/gameplay.png", renderer);
    imagenJugador = cargarLaImagen("assets/img/player.png", renderer);
    imagenIA = cargarLaImagen("assets/img/enemy.png", renderer);
    imagenPelota = cargarLaImagen("assets/img/ball.png", renderer);

    player_init(0, 300);
    ia_init(1230, 300);
    ball_init(640, 360);
}

void runGame() {
    initGame();
    Uint32 lastTick = SDL_GetTicks();

    while (isGameRunning) {
        Uint32 currentTick = SDL_GetTicks();
        float deltaTime = (currentTick - lastTick) / 1000.0f;
        lastTick = currentTick;

        processInput();
        updateGame(deltaTime);
        render();
    }

    SDL_DestroyTexture(imagenJugador);
    SDL_DestroyTexture(imagenIA);
    SDL_DestroyTexture(imagenPelota);
    SDL_DestroyTexture(imagenFondoJuego);
    SDL_DestroyTexture(imagenFondoMenu);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
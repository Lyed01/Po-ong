#include <iostream>
#include <SDL_ttf.h>
#include "../Render/render.h"
#include "../Input/input.h"
#include "../Ball/ball.h"
#include "../IA/ia.h"
#include "../Player/player.h"
#include "../globals.h"
#include <SDL_mixer.h>
#include <fstream>

using namespace std;

// Definiciones reales

SDL_Renderer* renderer = nullptr;
SDL_Window* window = nullptr;
SDL_Texture* imagenFondoJuego = nullptr;
SDL_Texture* imagenFondoMenu = nullptr;
SDL_Texture* imagenVictoria = nullptr;
SDL_Texture* imagenDerrota = nullptr;
SDL_Texture* imagenEmpate = nullptr;
SDL_Texture* imagenJugador = nullptr;
SDL_Texture* imagenIA = nullptr;
SDL_Texture* imagenPelota = nullptr;
Mix_Music* musicaFondo = nullptr;
const int NUM_SONIDOAGUA = 12;
Mix_Chunk* sonidosAgua[NUM_SONIDOAGUA];
Mix_Chunk* sonidoPunto = nullptr;
SDL_Texture* imagenPelotaHabilidad = nullptr;
TTF_Font* fuente;
TTF_Font* fuenteGrande;
TTF_Font* fuenteMediana;
SDL_Surface* textoSurface;
SDL_Texture* textoTexture;

SDL_Color color = { 0,0,0 };
Player player;
Player ia;
Ball ball;


//VARIABLES DE JUEGO
bool enPausa = false;
bool isGameRunning = true;
float timerPausa = 0.0f;
int puntosJugador = 0;
int puntosIA = 0;
bool pantallaDeMenu = true;
bool pantallaDeJuego = false;
bool pantallaDeResultado = false;
int contador;              // Duración inicial del contador en segundos
float tiempoParaActualizar = 1; // 1 segundo para decrementar
bool resultado = false;
bool esJugador = false;
enum Dificultad { FACIL, DIFICIL };
Dificultad dificultadSeleccionada = FACIL;



//VARIABLES PELOTA
bool pelotaConHabilidad = false;
float tiempoHabilidad = 0.0f;
const float DURACION_HABILIDAD = 3.0f;  // Segundos que dura el efecto en la pelota
float cooldownHabilidad = 0.0f;
const float COOLDOWN_HABILIDAD = 5.0f;  // Segundos que debe esperar el jugador para volver a activar


const float duracionPausa = 2.0f; // segundos de pausa tras un punto

//VARIABLES IA
bool iaModoFullTracking = false;
float iaModeTimer = 0.0f;
bool iaEnDash = false;
bool iaVolviendoDash = false;
float iaPosXOriginal = 0.0f;
float iaDashCooldown = 7.0f;
float iaDashTimer = 0.0f;


void ball_init(float x, float y) {
    ball.posX = x;
    ball.posY = y;
    ball.velocidadX = 500;
    ball.velocidadY = 300;
    ball.width = 75;
    ball.height = 75;
}//inicializar objeto y caracteristicas
void player_init(float x, float y) {
    player.posX = x;
    player.posY = y;
    player.width = 100;
    player.height = 200;
    player.speed = 1000;
}//inicializar objeto y caracteristicas
void ia_init(float x, float y) {
    ia.posX = x;
    ia.posY = y;
    ia.width = 100;
    ia.height = 200;
    ia.speed = 600;
}//inicializar objeto y caracteristicas

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

void aplicarHabilidadEnPelota() {
    pelotaConHabilidad = true;
    ball.width = 200;
    ball.height = 200;
}

bool checkPaddleCollision(Player& paddle, Ball& ball, bool isPlayer) {
    if (ball.posX <= paddle.posX + paddle.width &&
        ball.posX + ball.width >= paddle.posX &&
        ball.posY + ball.height >= paddle.posY &&
        ball.posY <= paddle.posY + paddle.height) {

        int indice = rand() % NUM_SONIDOAGUA;
        if (sonidosAgua[indice]) {
            Mix_PlayChannel(-1, sonidosAgua[indice], 0);
        }

        const float VELOCIDAD_INCREMENTO = 1.0008f;
        ball.velocidadX = -ball.velocidadX * VELOCIDAD_INCREMENTO;

        if (isPlayer) {
            ball.posX = paddle.posX + paddle.width;
        }
        else {
            ball.posX = paddle.posX - ball.width;
        }

        float paletaCentroY = paddle.posY + paddle.height / 2.0f;
        float ballCentroY = ball.posY + ball.height / 2.0f;
        float diffY = ballCentroY - paletaCentroY;
        ball.velocidadY = diffY * 7;

        if (paddle.special) {
            aplicarHabilidadEnPelota();
            paddle.special = false; // resetear el estado de habilidad
        }

        return true;
    }
    return false;
}

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

    if (dificultadSeleccionada == 1){
        ia.speed = 700;
        if (iaModeTimer >= 7.0f ) {
            iaModoFullTracking = true; // (rand() % 2 == 0);
            iaModeTimer = 0.0f;
        }
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

void renderContador() {
    if (contador < 0) return;  // No mostrar si es negativo

    string texto = to_string(contador);
    SDL_Surface* surface = TTF_RenderText_Solid(fuente, texto.c_str(), { 255, 255, 255 });

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    int texW = 0, texH = 0;
    SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);

    // Centrar arriba: x = (ancho_ventana - ancho_texto)/2, y = 20 px desde arriba
    SDL_Rect dstRect = { (1280 - texW) / 2, 20, texW, texH };

    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void mostrarPuntajes(TTF_Font* fuente, int puntaje, bool esJugador) {
    string texto = to_string(puntaje);
    textoSurface = TTF_RenderText_Solid(fuente, texto.c_str(), {250,250,250});
    textoTexture = SDL_CreateTextureFromSurface(renderer, textoSurface);

    int texW = 0, texH = 0;

    SDL_QueryTexture(textoTexture, nullptr, nullptr, &texW, &texH);
    SDL_Rect dstRect = { 1200, 20, texW, texH };
    if (esJugador) {
        dstRect = { 40, 20, texW, texH };
    }
 
    SDL_RenderCopy(renderer, textoTexture, nullptr, &dstRect);
    SDL_FreeSurface(textoSurface);
    SDL_DestroyTexture(textoTexture);
}

void render() {
    SDL_SetRenderDrawColor(renderer, 120, 140, 220, 200);
    SDL_RenderClear(renderer);

    if (pantallaDeJuego) {
        mostrarLaImagen(imagenFondoJuego, 0, 0, 1280, 720, renderer);

        if (pelotaConHabilidad) {
            mostrarLaImagen(imagenPelotaHabilidad, (int)ball.posX, (int)ball.posY, ball.width, ball.height, renderer);
        }
        else {
            mostrarLaImagen(imagenPelota, (int)ball.posX, (int)ball.posY, ball.width, ball.height, renderer);
        }

        mostrarLaImagen(imagenJugador, (int)player.posX, (int)player.posY, player.width, player.height, renderer);
        mostrarLaImagen(imagenIA, (int)ia.posX, (int)ia.posY, ia.width, ia.height, renderer);
        mostrarPuntajes(fuente, puntosIA, false);
        mostrarPuntajes(fuente, puntosJugador, true);
        renderContador();
    }
    else if (pantallaDeMenu) {
        mostrarLaImagen(imagenFondoMenu, 0, 0, 1280, 720, renderer);

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        bool mouseClick = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);

        // ---- BOTÓN "JUGAR" ----
        string textoJugar = "Jugar";
        SDL_Color colorJugar = { 0, 0, 0 };
        int texW = 0, texH = 0;
        TTF_SizeText(fuenteMediana, textoJugar.c_str(), &texW, &texH);
        SDL_Rect rectJugar = { 100, 500, texW, texH };

        if (mouseX >= rectJugar.x && mouseX <= rectJugar.x + rectJugar.w &&
            mouseY >= rectJugar.y && mouseY <= rectJugar.y + rectJugar.h) {
            colorJugar = { 255, 100, 100 }; // Hover
            if (mouseClick) {
                pantallaDeResultado = false;
                pantallaDeMenu = false;
                pantallaDeJuego = true;
                contador = 3;
            }
        }

        textoSurface = TTF_RenderText_Solid(fuenteMediana, textoJugar.c_str(), colorJugar);
        textoTexture = SDL_CreateTextureFromSurface(renderer, textoSurface);
        SDL_RenderCopy(renderer, textoTexture, nullptr, &rectJugar);
        SDL_FreeSurface(textoSurface);
        SDL_DestroyTexture(textoTexture);

        // ---- SELECTOR DE DIFICULTAD ----
        SDL_Color colorFacil = { 0, 0, 0 };
        SDL_Color colorDificil = { 0, 0, 0 };

        SDL_Rect rectFacil = { 800, 480, };
        SDL_Rect rectDificil = { 800, 580, };

        TTF_SizeText(fuenteMediana, "Facil", &rectFacil.w, &rectFacil.h);
        TTF_SizeText(fuenteMediana, "Dificil", &rectDificil.w, &rectDificil.h);

        if (mouseX >= rectFacil.x && mouseX <= rectFacil.x + rectFacil.w &&
            mouseY >= rectFacil.y && mouseY <= rectFacil.y + rectFacil.h) {
            colorFacil = { 255, 100, 100 };
            if (mouseClick) dificultadSeleccionada = FACIL;
        }

        if (mouseX >= rectDificil.x && mouseX <= rectDificil.x + rectDificil.w &&
            mouseY >= rectDificil.y && mouseY <= rectDificil.y + rectDificil.h) {
            colorDificil = { 255, 100, 100 };
            if (mouseClick) dificultadSeleccionada = DIFICIL;
        }

        // Color de selección activa
        if (dificultadSeleccionada == FACIL) colorFacil = { 0, 180, 0 };
        if (dificultadSeleccionada == DIFICIL) colorDificil = { 0, 180, 0 };

        textoSurface = TTF_RenderText_Solid(fuenteMediana, "Facil", colorFacil);
        textoTexture = SDL_CreateTextureFromSurface(renderer, textoSurface);
        SDL_RenderCopy(renderer, textoTexture, nullptr, &rectFacil);
        SDL_FreeSurface(textoSurface);
        SDL_DestroyTexture(textoTexture);

        textoSurface = TTF_RenderText_Solid(fuenteMediana, "Dificil", colorDificil);
        textoTexture = SDL_CreateTextureFromSurface(renderer, textoSurface);
        SDL_RenderCopy(renderer, textoTexture, nullptr, &rectDificil);
        SDL_FreeSurface(textoSurface);
        SDL_DestroyTexture(textoTexture);
    }
    else if (pantallaDeResultado) {

        if (puntosJugador > puntosIA) {
            mostrarLaImagen(imagenVictoria, 0, 0, 1280, 720, renderer);
            textoSurface = TTF_RenderText_Solid(fuenteGrande, "Victoria", { 250,250,250 });
            textoTexture = SDL_CreateTextureFromSurface(renderer, textoSurface);
            int texW = 100, texH = 100;
            SDL_QueryTexture(textoTexture, nullptr, nullptr, &texW, &texH);
            SDL_Rect dstRect = { 250, 100, texW, texH };

            SDL_RenderCopy(renderer, textoTexture, nullptr, &dstRect);
            SDL_FreeSurface(textoSurface);
            SDL_DestroyTexture(textoTexture);

        }
        else if(puntosJugador == puntosIA) {
            mostrarLaImagen(imagenEmpate, 0, 0, 1280, 720, renderer);
            textoSurface = TTF_RenderText_Solid(fuenteGrande, "Empate", { 250,250,250 });
            textoTexture = SDL_CreateTextureFromSurface(renderer, textoSurface);
            int texW = 100, texH = 100;
            SDL_QueryTexture(textoTexture, nullptr, nullptr, &texW, &texH);
            SDL_Rect dstRect = { 300, 100, texW, texH };

            SDL_RenderCopy(renderer, textoTexture, nullptr, &dstRect);
            SDL_FreeSurface(textoSurface);
            SDL_DestroyTexture(textoTexture);
        }

        else {
            mostrarLaImagen(imagenDerrota, 0, 0, 1280, 720, renderer);
            textoSurface = TTF_RenderText_Solid(fuenteGrande, "Derrota", {250,250,250});
            textoTexture = SDL_CreateTextureFromSurface(renderer, textoSurface);
            

            int texW = 100, texH = 100;
            SDL_QueryTexture(textoTexture, nullptr, nullptr, &texW, &texH);
            SDL_Rect dstRect = {  250, 150, texW, texH  };

            SDL_RenderCopy(renderer, textoTexture, nullptr, &dstRect);
            SDL_FreeSurface(textoSurface);
            SDL_DestroyTexture(textoTexture);
        }
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        bool mouseClick = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);

        string textoJugar = "Volver Al Menu";
        SDL_Color colorJugar = { 0, 0, 0 };
        int texW = 0, texH = 0;
        TTF_SizeText(fuenteMediana, textoJugar.c_str(), &texW, &texH);
        SDL_Rect rectJugar = { 100, 600, texW, texH };

        if (mouseX >= rectJugar.x && mouseX <= rectJugar.x + rectJugar.w &&
            mouseY >= rectJugar.y && mouseY <= rectJugar.y + rectJugar.h) {
            colorJugar = { 255, 100, 100 };
            if (mouseClick) {
                pantallaDeJuego = false;
                pantallaDeResultado = false;
                pantallaDeMenu = true;
            }
        }

        textoSurface = TTF_RenderText_Solid(fuenteMediana, textoJugar.c_str(), colorJugar);
        textoTexture = SDL_CreateTextureFromSurface(renderer, textoSurface);
        SDL_RenderCopy(renderer, textoTexture, nullptr, &rectJugar);
        SDL_FreeSurface(textoSurface);
        SDL_DestroyTexture(textoTexture);
        
    };
    SDL_RenderPresent(renderer);
}

static void updateGame(float deltaTime) {
    const float VELOCIDAD_INCREMENTO = 1.0008f;
    static float iaReactionTimer = 0.0f;
  

     if (pantallaDeJuego) {
        if (contador > 0) {
            tiempoParaActualizar -= deltaTime;
            if (tiempoParaActualizar <= 0.0f) {
                contador--;
                tiempoParaActualizar = 1.0f;
            }
        }
        if (contador == 0) {
            pantallaDeJuego = false;
            pantallaDeResultado = true;
        }
        if (enPausa) {
            timerPausa -= deltaTime;
            if (timerPausa <= 0.0f) {
                enPausa = false;
            }
            return; // Salimos sin actualizar movimientos ni colisiones
        }
        renderContador();
        movement(deltaTime);
        checkPoints(1280, 720);
        checkCollisions(720, 1280);
        updateIA(deltaTime, 640);
        cout << iaModoFullTracking;
        checkPaddleCollision(player, ball, true);
        checkPaddleCollision(ia, ball, false);


    }

    else if (pantallaDeResultado) {};
    
}

static void initGame() {

    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
   

    if (Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3) == 0) {
        cerr << "Error en Mix_Init: " << Mix_GetError() << endl;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cerr << "Error al inicializar SDL_mixer: " << Mix_GetError() << endl;
    }
    window = SDL_CreateWindow("Pong", 50, 50, 1280, 720, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
    imagenPelotaHabilidad = cargarLaImagen("assets/img/Puffer_Hit.png", renderer);
    imagenFondoJuego = cargarLaImagen("assets/img/gameplay.png", renderer);
    imagenJugador = cargarLaImagen("assets/img/Player_Squid.png", renderer);
    imagenIA = cargarLaImagen("assets/img/IA_Squid.png", renderer);
    imagenPelota = cargarLaImagen("assets/img/Puffer_Normal.png", renderer);
    imagenFondoMenu = cargarLaImagen("assets/img/menu.png", renderer);
    imagenVictoria = cargarLaImagen("assets/img/Victoria.png", renderer);
    imagenDerrota = cargarLaImagen("assets/img/Derrota.png", renderer);
    imagenEmpate = cargarLaImagen("assets/img/Empate.png", renderer);
    fuente = TTF_OpenFont("assets/fonts/team401.TTF", 20);
    fuenteMediana = TTF_OpenFont("assets/fonts/team401.TTF", 40);
    fuenteGrande = TTF_OpenFont("assets/fonts/team401.TTF", 80);
    player_init(0, 300);
    ia_init(1200, 300);
    ball_init(640, 360);

    
    musicaFondo = Mix_LoadMUS("assets/audio/Submerged.ogg");

    for (int i = 0; i < NUM_SONIDOAGUA; ++i) {
        string ruta = "assets/audio/SONIDOAGUA" + to_string(i + 1) + ".wav";
        sonidosAgua[i] = Mix_LoadWAV(ruta.c_str());
        if (!sonidosAgua[i]) {
            cerr << "Error cargando " << ruta << ": " << Mix_GetError() << endl;
        }
    }
    sonidoPunto = Mix_LoadWAV("assets/audio/SONIDOAGUA12.wav");

    if (musicaFondo) Mix_PlayMusic(musicaFondo, -1); // -1 = loop infinito
 
    
    // Ajustar volumen de música y efectos
    Mix_VolumeMusic(40); // Música al 30% aprox

    for (int i = 0; i < NUM_SONIDOAGUA; ++i) {
        if (sonidosAgua[i]) {
            Mix_VolumeChunk(sonidosAgua[i], 50); // FX al ~50-60%
        }
    }
    if (sonidoPunto) {
        Mix_VolumeChunk(sonidoPunto, 70);
    }
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
    Mix_FreeMusic(musicaFondo);
    for (int i = 0; i < NUM_SONIDOAGUA; ++i) {
        if (sonidosAgua[i]) {
            Mix_FreeChunk(sonidosAgua[i]);
        }
    }
    Mix_FreeChunk(sonidoPunto);
    Mix_CloseAudio();
    SDL_Quit();
}
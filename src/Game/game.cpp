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
SDL_Texture* imagenFondoMenu = nullptr;
SDL_Texture* imagenFondoJuego = nullptr;
SDL_Texture* imagenVictoria = nullptr;
SDL_Texture* imagenDerrota = nullptr;
SDL_Texture* imagenEmpate = nullptr;
SDL_Texture* imagenJugador = nullptr;
SDL_Texture* imagenIA = nullptr;
SDL_Texture* imagenPelota = nullptr;
SDL_Texture* imagenPelotaHabilidad = nullptr;


TTF_Font* fuente;
TTF_Font* fuenteGrande;
TTF_Font* fuenteMediana;
SDL_Surface* textoSurface;
SDL_Texture* textoTexture;

Mix_Music* musicaFondo = nullptr;
const int NUM_SONIDOAGUA = 12;
Mix_Chunk* sonidosAgua[NUM_SONIDOAGUA];
Mix_Chunk* sonidoPunto = nullptr;

Player player;
Player ia;
Ball ball;

//VARIABLES DE JUEGO
bool isGameRunning = true;
int contador;
bool pantallaDeMenu = true;
bool pantallaDeJuego = false;
bool pantallaDeResultado = false;
int puntosJugador = 0;
int puntosIA = 0;
bool resultado = false;
bool esJugador = false;
bool enPausa = false;
float timerPausa = 0.0f;
float tiempoParaActualizar = 1;
enum Dificultad { FACIL, DIFICIL };
Dificultad dificultadSeleccionada = FACIL;
const float duracionPausa = 0.5f;

//VARIABLES IA
bool iaModoFullTracking = false;
float iaModeTimer = 0.0f;
bool iaEnDash = false;
bool iaVolviendoDash = false;
float iaPosXOriginal = 0.0f;
float iaDashCooldown = 7.0f;
float iaDashTimer = 0.0f;

void ia_init(float x, float y) {
    ia.posX = x;
    ia.posY = y;
    ia.width = 100;
    ia.height = 200;
    ia.speed = 500;
}

void updateIA(float deltaTime, int canchaMitadX) {
    // Control del cambio de modo 
    iaModeTimer += deltaTime;

    if (dificultadSeleccionada == 1) {
        ia.speed = 700;
        if (iaModeTimer >= 7.0f) {
            iaModoFullTracking =  (rand() % 2 == 0);
            iaModeTimer = 0.0f;
        }
    }
    // cooldown de dash 
    if (iaDashTimer > 0.0f) {
        iaDashTimer -= deltaTime;
        if (iaDashTimer < 0.0f)
            iaDashTimer = 0.0f;
    }

    // Ver si puede dash 
    bool puedeDash = (iaDashTimer == 0.0f) && !iaEnDash && !iaVolviendoDash;
    bool pelotaCercaX = (ball.posX > ia.posX - 100) && (ball.posX < ia.posX + ia.width + 50);
    bool pelotaEnRangoY = (ball.posY + ball.height > ia.posY) && (ball.posY < ia.posY + ia.height);

    if (puedeDash && pelotaCercaX && pelotaEnRangoY) {
        iaEnDash = true;
        iaPosXOriginal = ia.posX;
        iaDashTimer = iaDashCooldown;
    }

    if (iaEnDash) {
        ball.aplicarHabilidad(deltaTime);
        ia.special = true;
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

    // tracking
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

    // Oscilacion 
    static float oscilacionTimer = 0.0f;
    oscilacionTimer += deltaTime;
    float oscilacion = sin(oscilacionTimer * 2.0f) * 100.0f;
    ia.posY += oscilacion * deltaTime;

    // Limitar dentro de la pantalla 
    if (ia.posY < 0) ia.posY = 0;
    if (ia.posY + ia.height > 720) ia.posY = 720 - ia.height;
}

bool checkPaddleCollision(Player& paddle, Ball& ball, bool isPlayer, float deltatime) {
    if (ball.posX <= paddle.posX + paddle.width &&
        ball.posX + ball.width >= paddle.posX &&
        ball.posY + ball.height >= paddle.posY &&
        ball.posY <= paddle.posY + paddle.height) {

        int indice = rand() % NUM_SONIDOAGUA;
        if (sonidosAgua[indice]) {
            Mix_PlayChannel(-1, sonidosAgua[indice], 0);
        }

        const float VELOCIDAD_INCREMENTO = 1.01f;
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

        if (paddle.enDash) {
            ball.aplicarHabilidad(deltatime);
           
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

void checkPoints(int ventanaAncho, int ventanaAlto) {
    if (ball.posX + ball.width < 0) {
        puntosIA++;
        Mix_PlayChannel(-1, sonidoPunto, 0);
        resetPositionsAndPause(ventanaAncho, ventanaAlto, false);
        cout << puntosIA;
    }
    if (ball.posX > ventanaAncho) {
        puntosJugador++;
        Mix_PlayChannel(-1, sonidoPunto, 0);
        resetPositionsAndPause(ventanaAncho, ventanaAlto, true);

    }
}

void render() {

    SDL_SetRenderDrawColor(renderer, 120, 140, 220, 200);
    SDL_RenderClear(renderer);

    if (pantallaDeJuego) {
        mostrarLaImagen(imagenFondoJuego, 0, 0, 1280, 720, renderer);

        if (ball.habilidad) {
            mostrarLaImagen(imagenPelotaHabilidad, (int)ball.posX, (int)ball.posY, ball.width, ball.height, renderer);
        }
        else {
            mostrarLaImagen(imagenPelota, (int)ball.posX, (int)ball.posY, ball.width, ball.height, renderer);
        }

        mostrarLaImagen(imagenJugador, (int)player.posX, (int)player.posY, player.width, player.height, renderer);
        mostrarLaImagen(imagenIA, (int)ia.posX, (int)ia.posY, ia.width, ia.height, renderer);
        mostrarPuntajes(fuente, puntosIA, false);
        mostrarPuntajes(fuente, puntosJugador, true);
        if (contador < 0) return; 

        string texto = to_string(contador);
        renderTexto(texto, 620, 20, {250,250,250}, fuente);
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
            renderTexto("Victoria", 250, 150, { 250,250,250 }, fuenteGrande);

        }
        else if(puntosJugador == puntosIA) {
            mostrarLaImagen(imagenEmpate, 0, 0, 1280, 720, renderer);
            renderTexto("Empate", 300, 150, { 250,250,250 }, fuenteGrande);
        }

        else { 
            mostrarLaImagen(imagenDerrota, 0, 0, 1280, 720, renderer);
            renderTexto("Derrota", 250, 150, { 250,250,250 }, fuenteGrande);
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
    player.init(0, 300);
    ia_init(1200, 300);
    ball.init(640, 360);



    musicaFondo = Mix_LoadMUS("assets/audio/Submerged.ogg");

    for (int i = 0; i < NUM_SONIDOAGUA; ++i) {
        string ruta = "assets/audio/SONIDOAGUA" + to_string(i + 1) + ".wav";
        sonidosAgua[i] = Mix_LoadWAV(ruta.c_str());
        if (!sonidosAgua[i]) {
            cerr << "Error cargando " << ruta << ": " << Mix_GetError() << endl;
        }
    }
    sonidoPunto = Mix_LoadWAV("assets/audio/short-quick-dive.wav");

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

static void updateGame(float deltaTime) {  
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
            return; 
        }

        movement(deltaTime);
        checkPoints(1280, 720);
        updateIA(deltaTime, 850);
        checkPaddleCollision(player, ball, true, deltaTime);
        checkPaddleCollision(ia, ball, false, deltaTime);
     

    }
     else if (pantallaDeMenu) {
         ball.init(640, 360);
         puntosJugador, puntosIA = 0;
         ball.habilidad = false;
         contador = 30;
         resetPositionsAndPause(1280, 720, true);
    };
    
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

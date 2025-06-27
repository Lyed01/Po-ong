#include "player.h"

void Player::init(float x, float y, int w, int h, float s) {
    posX = x;
    posY = y;
    width = w;
    height = h;
    speed = s;
}

void Player::move(float deltaTime, int screenHeight) {
    // === Detección de Dash ===
    if (dashTimer > 0.0f) {
        dashTimer -= deltaTime;
        if (dashTimer < 0.0f) dashTimer = 0.0f;
    }

    if (special && !enDash && !volviendoDash && dashTimer == 0.0f) {
        enDash = true;
        posXOriginal = posX;
        dashTimer = dashCooldown;
    }

    // === Movimiento Normal ===
    if (!enDash && !volviendoDash) {
        if (up) posY -= speed * deltaTime;
        if (down) posY += speed * deltaTime;
    }

    // === Dash Activo ===
    if (enDash) {
        posX += speed * 1.5f * deltaTime;
        if (posX >= posXOriginal + 150) {
            enDash = false;
            volviendoDash = true;
        }
    }
    else if (volviendoDash) {
        posX -= speed * 0.5f * deltaTime;
        if (posX <= posXOriginal) {
            posX = posXOriginal;
            volviendoDash = false;
        }
    }

    // === Límite pantalla ===
    if (posY < 0) posY = 0;
    if (posY + height > screenHeight)
        posY = screenHeight - height;
}

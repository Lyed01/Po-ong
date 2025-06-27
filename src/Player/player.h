#pragma once

struct Player {
    float posX, posY;
    int width, height;
    float speed;
    bool up = false, down = false, special = false;

    // Dash
    bool enDash = false;
    bool volviendoDash = false;
    float dashCooldown = 7.0f;
    float dashTimer = 0.0f;
    float posXOriginal = 0.0f;

    void init(float x, float y, int w = 100, int h = 200, float s = 1000.0f);
    void move(float deltaTime, int screenHeight);
};

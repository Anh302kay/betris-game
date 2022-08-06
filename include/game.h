#pragma once
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "renderwindow.h"
#include "utils.h"

struct keyDelay
{
    //key delay default is 1
    float delay = 1;
    //timer has to be over delay in order for keypress to register
    float timer = 0.f;
};

class Game
{
public:
    void init();

    void run();

    void cleanup();

private:

    bool KeyPressed(int key, double &deltaTime);
    void pollInput();
    void drawGrid();
    void update();
    void isOver();
    void displayGameOver();
    void setGhostTetromino();
    void renderChar(const char text, int x, int y, SDL_Colour colour);
    void renderText(const char* text, int x, int y, float scale, SDL_Colour colour);

    RenderWindow Window;
    SDL_Event mainEvent;
    bool running = true;

    keyDelay kDelay[284];
    Uint64 currentTick = SDL_GetPerformanceCounter();
    Uint64 lastTick = 0;
    double deltaTime = 0;
};
#include <iostream>
#include <sstream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "game.h"
#include "renderwindow.h"
#include "text.h"
#include "utils.h"
#include "sprite.h"
#include "entity.h"

extern SDL_Renderer* gRenderer;

static bool menu = true;

static const int gridXOffset = 144;
static const int gridYOffset = 10;
static const int gridWidth = 11;
static const int gridHeight = 20;

static char gridArray[gridWidth* gridHeight];

static int scale = 1;

static float timeUntilFall = 100;
static float timeUntilReset = 100;

static Tetromino currentTetromino;
static Tetromino ghostTetromino;
static Tetromino holdTetromino;
static Tetromino nextTetrominos[5];

static bool holdLock = false;

static int score = 0;
static int lines = 0;

static bool over = false;

static Mix_Music* theme;

static Mix_Chunk *down;
static Mix_Chunk *line;

#ifdef __EMSCRIPTEN__
static SDL_Texture *grid;
static SDL_Texture *scorebox;
#endif


bool Game::KeyPressed(int key, double &deltaTime)
{
    const Uint8 *keyState = SDL_GetKeyboardState(NULL);
    if (keyState[key])
    {
        kDelay[key].timer += 0.01 * deltaTime;
        if (kDelay[key].timer > kDelay[key].delay)
        {
            kDelay[key].timer = 0;
            return true;
        }
    }
    else
    {
        kDelay[key].timer = kDelay[key].delay + 1;
    }

    return false;
}

void Game::init()
{
    Window = RenderWindow("test", 665, 672);
    randomiseTetromino(currentTetromino);
    holdTetromino.x = 10;
    holdTetromino.y = gridYOffset + 31;
    holdTetromino.tetromino = "................";
    kDelay[SDL_SCANCODE_R].delay = 3;
    kDelay[SDL_SCANCODE_W].delay = 3;
    kDelay[SDL_SCANCODE_UP].delay = 3;
    kDelay[SDL_SCANCODE_S].delay = 1.5;
    kDelay[SDL_SCANCODE_DOWN].delay = 1.5;
    kDelay[SDL_SCANCODE_SPACE].delay = 10;
    kDelay[SDL_SCANCODE_H].delay = 10;
    for(char& ch : gridArray)
    {
        ch = '0';
    }

    for (int i = 0; i < 5; i++)
    {
        randomiseTetromino(nextTetrominos[i]);
        nextTetrominos[i].pos = {(gridXOffset) + gridWidth * 31 + 10, (float)gridYOffset + (float)(gridYOffset + 31 * (int)(3 * i) + 31)};
    }
    SDL_Surface* icon = IMG_Load("icon.png");
    Window.setIcon(icon);
    SDL_FreeSurface(icon);

    theme = Mix_LoadMUS("res/sound/theme.wav");
    down = Mix_LoadWAV("res/sound/down.wav");
    line = Mix_LoadWAV("res/sound/line.wav");
    Mix_PlayMusic(theme, -1);
    Mix_VolumeMusic(25);

#ifdef __EMSCRIPTEN__
    grid = Window.loadTexture("res/gfx/grid.png");
    scorebox = Window.loadTexture("res/gfx/scorebox.png");
#endif
}

void Game::pollInput()
{
    SDL_PumpEvents();
    while (SDL_PollEvent(&mainEvent))
    {
        if (mainEvent.type == SDL_QUIT)
            running = false;

        Window.handleEvents(mainEvent);
    }
    if(KeyPressed(SDL_SCANCODE_S, deltaTime) || KeyPressed(SDL_SCANCODE_DOWN, deltaTime))
    {
        currentTetromino.y += (currentTetromino.h);
    }
    if(KeyPressed(SDL_SCANCODE_SPACE, deltaTime))
    {
        std::string piece;
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                piece += currentTetromino.tetromino[tetrominoRotateIndex(currentTetromino, x, y)];
            }
        }
        bool colide = false;
        //probably very ineffcient to do it this way but its called bad tetris for a reason
        while(!colide)
        {
            bool noColide = true;
            for (int i = 0; (size_t)i < piece.length(); i++)
            {
                if (piece[i] == 'X')
                {
                    int x1 = currentTetromino.x + currentTetromino.w * (i % 4);
                    int y1 = currentTetromino.y + currentTetromino.h * (floor(i / 4));
                    if (gridArray[((y1 - gridYOffset) / 31) * gridWidth + ((x1 - gridXOffset) / 31)] != '0')
                    {
                        noColide = false;
                    }
                }
            }
            if(noColide)
            {
                currentTetromino.y += (currentTetromino.h);
            }
            else
            {
                colide = true;
            }
            
        }
        //final check to see if any piece is coliding with another piece
        for (int i = 0; (size_t)i < piece.length(); i++)
        {
            if (piece[i] == 'X')
            {
                int x1 = currentTetromino.x + currentTetromino.w * (i % 4);
                int y1 = currentTetromino.y + currentTetromino.h * (floor(i / 4));
                if (gridArray[((y1 - gridYOffset) / 31) * gridWidth + ((x1 - gridXOffset) / 31)] != '0')
                {
                    currentTetromino.y -= (currentTetromino.h);
                }
            }
        }
    }

    if (KeyPressed(SDL_SCANCODE_H, deltaTime) || KeyPressed(SDL_SCANCODE_C, deltaTime))
    {
        if(holdTetromino.tetromino == "................" && !holdLock)
        {
            holdTetromino.tetromino = currentTetromino.tetromino;
            holdTetromino.colour = currentTetromino.colour;

            currentTetromino.tetromino = nextTetrominos[0].tetromino;
            currentTetromino.rotation = nextTetrominos[0].rotation;
            currentTetromino.colour = nextTetrominos[0].colour;
            currentTetromino.pos = {268, 10};

            for (int i = 0; i < 4; i++)
            {
                nextTetrominos[i].tetromino = nextTetrominos[i + 1].tetromino;
                nextTetrominos[i].colour = nextTetrominos[i + 1].colour;
            }
            randomiseTetromino(nextTetrominos[4]);
            nextTetrominos[4].rotation = 0;
            holdLock = true;
        }
        else if (!holdLock)
        {
            Tetromino t = currentTetromino;
            currentTetromino.tetromino = holdTetromino.tetromino;
            currentTetromino.rotation = 0;
            currentTetromino.colour = holdTetromino.colour;
            currentTetromino.pos = {268, 10};

            holdTetromino.tetromino = t.tetromino;
            holdTetromino.rotation = 0;
            holdTetromino.colour = t.colour;
            holdLock = true;
        }
        
    }
    
    
    //jank workaround so that we can't move through blocks
    if (KeyPressed(SDL_SCANCODE_A, deltaTime) || KeyPressed(SDL_SCANCODE_LEFT, deltaTime))
    {
        std::string piece;
        bool pressed = false;
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                piece += currentTetromino.tetromino[tetrominoRotateIndex(currentTetromino, x, y)];
            }
        }
        for (int i = 0; (size_t)i < piece.length(); i++)
        {
            if (piece[i] == 'X')
            {
                int x1 = currentTetromino.x + currentTetromino.w * (i % 4);
                int y1 = currentTetromino.y + currentTetromino.h * (floor(i / 4));
                if (gridArray[((y1 - gridYOffset) / 31) * gridWidth + ((x1 - gridXOffset) / 31)] != '0')
                {
                    currentTetromino.x += (currentTetromino.w);
                }
                else if (!pressed)
                {
                    currentTetromino.x -= (currentTetromino.w);
                    pressed = true;
                }
            }
        }
    }
    else if (KeyPressed(SDL_SCANCODE_D, deltaTime) || KeyPressed(SDL_SCANCODE_RIGHT, deltaTime))
    {
        std::string piece;
        bool pressed = false;
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                piece += currentTetromino.tetromino[tetrominoRotateIndex(currentTetromino, x, y)];
            }
        }
        for (int i = 0; (size_t)i < piece.length(); i++)
        {
            if (piece[i] == 'X')
            {
                int x1 = currentTetromino.x + currentTetromino.w * (i % 4);
                int y1 = currentTetromino.y + currentTetromino.h * (floor(i / 4));
                if(gridArray[((y1 - gridYOffset) / 31) * gridWidth + ((x1 - gridXOffset) / 31)] != '0')
                {
                    currentTetromino.x -= (currentTetromino.w);
                }
                else if (!pressed)
                {
                    currentTetromino.x += (currentTetromino.w);
                    pressed = true;
                }
            }
        }
       // currentTetromino.x += (currentTetromino.w);
    }

    if (KeyPressed(SDL_SCANCODE_R, deltaTime) || KeyPressed(SDL_SCANCODE_W, deltaTime) || KeyPressed(SDL_SCANCODE_UP, deltaTime))
    {
        rotateTetromino(currentTetromino, currentTetromino.rotation + 1);
    }

    if(KeyPressed(SDL_SCANCODE_1, deltaTime))
    {
        scale = 1;
        Window.setScale(1,1);
        Window.setWH(362, 672);
    }
    else if (KeyPressed(SDL_SCANCODE_2, deltaTime))
    {
        scale = 2;
        Window.setScale(1.5,1.5);
        Window.setWH(362*1.5, 672*1.5);
    }
}

void Game::renderChar(const char text, int x, int y, SDL_Colour colour)
{
    SDL_Texture *tex = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 8, 8);
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    char* letter = bitmap[(size_t)text];
    Uint32 pixels[8*8] = {0};
    Uint32 Pcolour = ((colour.r << 8) | (colour.g)) << 16 | ((colour.b << 8) | (colour.a));

    for(Uint32& pix : pixels)
    {
        pix = 0XFFFFFF00;
    }
    for(int yy = 0; yy < 8; yy++)
    {
        for(int xx = 0; xx < 8; xx++)
        {
            if(letter[yy] & (1 << xx))
            {
                pixels[(yy * 8) + xx] = Pcolour;
            }
        }
    }
    SDL_UpdateTexture(tex, NULL, pixels, 8 * 4);
    Window.render(tex, {x,y, 16, 16});
    SDL_DestroyTexture(tex);
}

void Game::renderText(const char* text, int x, int y, float scale, SDL_Colour colour)
{
    int len = strlen(text);
    SDL_Texture *tex = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 8 * len, 8);
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    Uint32 pixels[8 * 8 * len];
    memset(pixels, 0, 8 * 8 * len*sizeof(Uint32));
    Uint32 Pcolour = ((colour.r << 8) | (colour.g)) << 16 | ((colour.b << 8) | (colour.a));

    for (Uint32 &pix : pixels)
    {
        pix = 0XFFFFFF00;
    }
    for(int i = 0; i < len; i++)
    {
        char *letter = bitmap[(size_t)text[i]];
        for (int yy = 0; yy < 8; yy++)
        {
            for (int xx = 0; xx < 8; xx++)
            {
                if (letter[yy] & (1 << xx))
                {
                    pixels[(yy * 8 * len) + xx + (i * 8)] = Pcolour;
                }
            }
        }
    }

    SDL_UpdateTexture(tex, NULL, pixels, 8 * 4 * len);
    Window.render(tex, {x, y, (int)(8 * scale * len), (int)(8 * scale)});
    SDL_DestroyTexture(tex);
}

void Game::drawGrid()
{
#ifdef __EMSCRIPTEN__
    Window.render(grid, {gridXOffset, gridYOffset, 342, 652});
#else
    Window.drawRect(gridXOffset, 10, 32, 652, {0, 0, 0, 255});
    Window.drawRect(gridXOffset + (gridWidth * 31) - 31, gridYOffset, 32, 652, {0, 0, 0, 255});
    Window.drawRect(gridXOffset, 630, 342, 32, {0, 0, 0, 255});
    // draws the lines
    for (int i = 0; i < 22; i++)
    {
        if (i < 12)
            Window.drawLine(31 * i + gridXOffset, gridYOffset, 31 * i + gridXOffset, 651 + gridYOffset, {125, 125, 125, 255});
        Window.drawLine(gridXOffset, 31 * i + gridYOffset, 341 + gridXOffset, 31 * i + gridYOffset, {125, 125, 125, 255});
    }
#endif
    //draws the blocks that have been put down
    for(int y = 0; y < gridHeight; y++)
    {
        for(int x = 0; x < gridWidth; x++)
        {
            if ((gridArray[(y * gridWidth) + x]) != '0')
            {
                SDL_Colour colour = {63,63,63,255};
                if(!over)
                switch (gridArray[(y * gridWidth) + x])
                {
                case TETROMINO_I + 1:
                    colour = {0, 255, 255, 255};
                    break;
                case TETROMINO_J + 1:
                    colour = {0, 41, 248, 255};
                    break;
                case TETROMINO_L + 1:
                    colour = {255, 168, 48, 255};
                    break;
                case TETROMINO_O + 1:
                    colour = {255, 252, 71, 255};
                    break;
                case TETROMINO_S + 1:
                    colour = {0, 253, 69, 255};
                    break;
                case TETROMINO_T + 1:
                    colour = {151, 41, 248, 255};
                    break;
                case TETROMINO_Z + 1:
                    colour = {255, 0, 11, 255};
                    break;
                default:
                    break;
                }
                Window.drawRect({gridXOffset + (31 * x), gridYOffset + (31 * y),32,32}, colour);
            }
        }
    }
}

void Game::update()
{
    timeUntilFall -= 0.1 * deltaTime;
    if(timeUntilFall <= 0)
    {
        timeUntilFall = 60;
        currentTetromino.y += 31;
    }
}

void Game::isOver()
{
#ifdef __EMSCRIPTEN__
    // if (gridArray[15] != '0' || gridArray[16] != '0' || gridArray[17] != '0')
    // {
    //     Mix_HaltMusic();
    //     over = true;
    // }
#endif
    if (gridArray[4] != '0' || gridArray[5] != '0' || gridArray[6] != '0')
    {
        Mix_HaltMusic();
        over = true;
    }
}

void Game::setGhostTetromino()
{
    for(int i = 0; i < 33; i++)
    {
        if(gridArray[i] != '0')
            return;
    }
    ghostTetromino.tetromino = currentTetromino.tetromino;
    ghostTetromino.pos = currentTetromino.pos;
    ghostTetromino.colour = currentTetromino.colour;
    ghostTetromino.colour.a = 155;
    ghostTetromino.rotation = currentTetromino.rotation;
    std::string piece;
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            piece += ghostTetromino.tetromino[tetrominoRotateIndex(ghostTetromino, x, y)];
        }
    }
    bool colide = false;
    // probably very ineffcient to do it this way but its called bad tetris for a reason
    while (!colide)
    {
        bool noColide = true;
        for (int i = 0; (size_t)i < piece.length(); i++)
        {
            if (piece[i] == 'X')
            {
                int x1 = ghostTetromino.x + ghostTetromino.w * (i % 4);
                int y1 = ghostTetromino.y + ghostTetromino.h * (floor(i / 4));
                if (gridArray[((y1 - gridYOffset) / 31) * gridWidth + ((x1 - gridXOffset) / 31)] != '0')
                {
                    noColide = false;
                }
            }
        }
        if (noColide)
        {
            ghostTetromino.y += (ghostTetromino.h);
        }
        else
        {
            colide = true;
        }
    }
    // final check to see if any piece is coliding with another piece
    for (int i = 0; (size_t)i < piece.length(); i++)
    {
        if (piece[i] == 'X')
        {
            int x1 = ghostTetromino.x + ghostTetromino.w * (i % 4);
            int y1 = ghostTetromino.y + ghostTetromino.h * (floor(i / 4));
            if (gridArray[((y1 - gridYOffset) / 31) * gridWidth + ((x1 - gridXOffset) / 31)] != '0')
            {
                ghostTetromino.y -= (ghostTetromino.h);
            }
        }
    }

    for (int i = 0; (size_t)i < piece.length(); i++)
    {
        if (piece[i] == 'X')
        {
            int x1 = ghostTetromino.x + ghostTetromino.w * (i % 4);
            int y1 = ghostTetromino.y + ghostTetromino.h * (floor(i / 4));
            Window.drawRect({x1, y1, 32, 32}, {40, 100, 120, 255});
            Window.drawRect({(int)ghostTetromino.x + ghostTetromino.w * (i % 4), (int)ghostTetromino.y + (int)(31 * floor(i / 4)), ghostTetromino.w + 1, ghostTetromino.h + 1}, ghostTetromino.colour);
        }
    }
}

void Game::displayGameOver()
{
    if (timeUntilReset <= 0)
    {
#ifdef __EMSCRIPTEN__
        Window.render(scorebox, {100,100, 400, 200});
#else
        Window.drawRect({100, 100, 400, 200}, {30, 30, 30, 200});
        Window.drawRect({102, 102, 396, 196}, {60, 60, 60, 200});
#endif
        renderText("GAME OVER!", 140, 120, 4, {255,255,255,255});
        renderText("PRESS ENTER TO PLAY AGAIN", 150, 152, 1.5, {255,255,255,255});

        renderText(("SCORE:" + std::to_string(score)).c_str(), 110, 190, 3, {255, 255, 255, 255});
        renderText(("LINES:" + std::to_string(lines)).c_str(), 110, 230, 3, {255, 255, 255, 255});

        if(KeyPressed(SDL_SCANCODE_RETURN, deltaTime))
        {
            Mix_PlayMusic(theme, -1);
            score = 0;
            lines = 0;
            timeUntilFall = 100;
            timeUntilReset = 100;
            over = false;
            for (char &ch : gridArray)
            {
                ch = '0';
            }
            currentTetromino.pos = {268, 10};
            holdTetromino.tetromino = "................";
        }
    }
    
}

void Game::run()
{
#ifndef __EMSCRIPTEN__
    while(running)
    {
#endif
        lastTick = currentTick;
        currentTick = SDL_GetPerformanceCounter();
        deltaTime = (double)((currentTick - lastTick) * 1000 / (double)SDL_GetPerformanceFrequency());
        if(menu)
        {
            SDL_PumpEvents();
            while (SDL_PollEvent(&mainEvent))
            {
                if (mainEvent.type == SDL_QUIT)
                    running = false;

                Window.handleEvents(mainEvent);
            }
            if(KeyPressed(SDL_SCANCODE_RETURN, deltaTime))
            {
                menu = false;
                Mix_HaltMusic();
                Mix_PlayMusic(theme, -1);
            }

            
            Window.setDrawColour({40, 100, 120, 255});
            Window.clear();
            drawGrid();
            renderText("BETRIS", gridXOffset + (gridWidth*31)/2 - 137, 100, 6 ,{255,255,255,255});
            renderText("PRESS ENTER TO PLAY", gridXOffset + (gridWidth * 31) / 2 - 228, 160, 3, {255,255,255,255});
        }
        else
        {
            pollInput();
            isOver();
            if(!over)
            {
                update();
                tetrominoCollison(currentTetromino, nextTetrominos, down, line, score, lines, holdLock, gridXOffset, gridYOffset, gridWidth, gridHeight, gridArray);
            }
            else
            {
                timeUntilReset -= 0.1 * deltaTime;
            }
            

            Window.setDrawColour({40, 100, 120, 255});
            Window.clear();
            drawGrid();
            if(!over)
            {
                setGhostTetromino();
                Window.render(currentTetromino);
            }
            Window.render(holdTetromino);
            for (Tetromino &t : nextTetrominos)
            {
                Window.render(t);
            }
            renderText("HOLD", gridXOffset/2 - 32, 10, 2, {255, 255, 255, 255});
            renderText(("SCORE:" + std::to_string(score)).c_str(), gridXOffset + (gridWidth * 31) + 10, 500, 2, {255, 255, 255, 255});
            renderText(("LINES:" + std::to_string(lines)).c_str(), gridXOffset + (gridWidth * 31) + 10, 520, 2, {255, 255, 255, 255});
            renderText("NEXT", 521, 10, 2, {255,255,255,255});
            displayGameOver();
        }
        
        Window.display();
#ifndef __EMSCRIPTEN__
    }
#endif
}

void Game::cleanup()
{
    Mix_FreeMusic(theme);
    theme = nullptr;
    Window.cleanup();
}
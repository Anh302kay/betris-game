#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "utils.h"

enum
{
    TETROMINO_I,
    TETROMINO_J,
    TETROMINO_L,
    TETROMINO_O,
    TETROMINO_S,
    TETROMINO_T,
    TETROMINO_Z,
};

struct Tetromino
{
    Tetromino() {}
    std::string tetromino = "";
    SDL_Colour colour = {0,0,0,255};
    int rotation = 0;
    union 
    {
        struct 
        {
            float x, y;
        };
        Vector2f pos = {268, 10};
    };
    const int w = 31, h = 31;
};

void randomiseTetromino(Tetromino &tetromino);
void rotateTetromino(Tetromino &tetromino, int rotate);
int tetrominoRotateIndex(Tetromino &tetromino, int x, int y);
void tetrominoCollison(Tetromino &tetromino, Tetromino *nextTetromino, Mix_Chunk* down, Mix_Chunk* line, int &score, int &lines, bool &holdLock, int xOffset, int yOffset, int gridWidth, int gridHeight, char *gridArray);
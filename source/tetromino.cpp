#include <iostream>
#include <random>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "utils.h"
#include "tetromino.h"

//static std::string allTetromino[8] = {"..X...X...X...X.", "X...XXX.........", "...X.XXX........", ".XX..XX.........", ".XX.XX..........", "..X..XXX........", "XX...XX.........", "XXXXXX.XXXXXXX.X"};

//static std::string allTetromino[8] = {"........XXXX....", "....X...XXX.....", "......X.XXX.....", ".....XX..XX.....", ".....XX.XX......", ".....X..XXX.....", "....XX...XX....." "XXXXXX.XXXXXXX.X"};

// static std::string allTetromino[8] = {"..X...X...X...X.", "X...XXX.........", "..X.XXX.........", ".XX..XX.........", ".XX.XX..........", ".X..XXX.........", "XX...XX........."};

static std::string allTetromino[7] = {"....XXXX........", "X...XXX.........", "..X.XXX.........", ".XX..XX.........", ".XX.XX..........", ".X..XXX.........", "XX...XX........."};

void randomiseTetromino(Tetromino &tetromino)
{
    std::random_device rnd;
    std::mt19937 rng(rnd());
    std::uniform_int_distribution<int> dist7(0, 6);

    int piece = dist7(rng);
    tetromino.tetromino = allTetromino[piece];
    switch(piece)
    {
    case TETROMINO_I:
        tetromino.colour = {0, 255, 255, 255};
        tetromino.rotation = 0;
        break;
    case TETROMINO_J:
        tetromino.colour = {0, 41, 248, 255};
        tetromino.rotation = 0;
        break;
    case TETROMINO_L:
        tetromino.colour = {255, 168, 48, 255};
        tetromino.rotation = 0;
        break;
    case TETROMINO_O:
        tetromino.colour = {255, 252, 71, 255};
        tetromino.rotation = 0;
        break;
    case TETROMINO_S:
        tetromino.colour = {0, 253, 69, 255};
        tetromino.rotation = 0;
        break;
    case TETROMINO_T:
        tetromino.colour = {151, 41, 248, 255};
        tetromino.rotation = 0;
        break;
    case TETROMINO_Z:
        tetromino.colour = {255, 0, 11, 255};
        tetromino.rotation = 0;
        break;
    default:
        break;
    }
}
// jank way of rotating but it works
void rotateTetromino(Tetromino &tetromino, int rotate)
{
    if(tetromino.tetromino == allTetromino[TETROMINO_O])
        return;

    tetromino.rotation = rotate % 4;

    if(tetromino.tetromino == allTetromino[TETROMINO_I])
        return;

    switch (tetromino.rotation)
    {
        case 0:
            tetromino.y += tetromino.h;
            break;
        case 1:
            tetromino.x -= tetromino.w;
            break;
        case 2:
            tetromino.y -= tetromino.h;
            break;
        case 3:
            tetromino.x += tetromino.w;
            break;
        default:
            break;
    }
}

int tetrominoRotateIndex(Tetromino &tetromino, int x, int y)
{
    int index;
    switch(tetromino.rotation)
    {
        
        case 0:               
            index = y * 4 + x;
            break;            
        case 1:                      
            index = 12 + y - (x * 4);
            break;                   
        case 2:                      
            index = 15 - (y * 4) - x;
            break;                   
        case 3:                     
            index = 3 - y + (x * 4); 
            break;                  
    }                           
        return index;
}
//function used to be clean now its spaghetti
void tetrominoCollison(Tetromino &tetromino, Tetromino *nextTetromino, Mix_Chunk *down, Mix_Chunk *line, int &score, int &lines, bool &holdLock, int xOffset, int yOffset, int gridWidth, int gridHeight, char *gridArray)
{
    //tetromino.x = (tetromino.x + tetromino.w < xOffset) ? xOffset : tetromino.x;
    
    int width = gridWidth * 31;
    int height = gridHeight * 31;
    bool lock = false;
    std::string piece;
    //gets tetromino with correct rotation
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            piece += tetromino.tetromino[tetrominoRotateIndex(tetromino, x, y)];
        }
    }
    //checks if tetromino is out of bounds
    for (int i = 0; (size_t)i < piece.length(); i++)
    {
        if (piece[i] == 'X')
        {
            int x1 = tetromino.x + tetromino.w * (i % 4);
            int y1 = tetromino.y + tetromino.h * (floor(i / 4));
            if ((y1 - yOffset) / 31 < 0)
                return;

            tetromino.x = (x1 - tetromino.w < xOffset) ? tetromino.x + tetromino.w : tetromino.x;

            tetromino.x = (x1 + tetromino.w > width + xOffset - 31) ? tetromino.x - tetromino.w : tetromino.x;

            //tetromino.x = (gridArray[((y1 - yOffset) / 31) * gridWidth + ((x1 - xOffset) / 31)] != '0') ? tetromino.x + tetromino.w : tetromino.x;

            if(tetromino.y + tetromino.h * (floor(i / 4)) > height || gridArray[((y1 - yOffset) / 31) * gridWidth + ((x1 - xOffset) / 31)] != '0')
            {
                tetromino.y -= tetromino.h;
                lock = true;
            }
        }
    }

    if(!lock)
    {
        return;
    }
    else
    {
        int index = 0;
        Mix_PlayChannel(-1, down, 0);
        for(int i = 0; i < 7; i++)
        {
            if(tetromino.tetromino == allTetromino[i])
                index = i + 1;
        }
        for (int i = 0; (size_t)i < piece.length(); i++)
        {
            if (piece[i] == 'X')
            {
                int x1 = tetromino.x + tetromino.w * (i % 4);
                int y1 = tetromino.y + tetromino.h * (floor(i / 4));

                int x2 = (x1 - xOffset) / 31;
                int y2 = (y1 - yOffset) / 31;
                gridArray[(y2 * gridWidth) + x2] = index;
            }
        }
        int linesY[4] = {0,0,0,0};
        for(int i = 0; i < 4; i++)
        {
            int y = (tetromino.y + (tetromino.h * i) - yOffset) / 31;
            if(y < 0)
            {
                return;
            }
            else if(y < gridHeight)
            {
                // std::cout << y << "\n";
                bool line = true;
                for (int x = 1; x < gridWidth - 1; x++)
                    line &= (gridArray[(y) * gridWidth + x]) != '0';
                if(line)
                {
                    linesY[i] = y;
                }
            }
        }
        int removedLines = 0;
        for (int &i : linesY)
        {
            if(i != 0)
            {
                for (int y = i; y > 0; y--)
                {
                    for(int x = 1; x < gridWidth -1; x++)
                        gridArray[y * gridWidth + x] = gridArray[(y - 1) * gridWidth + x];
                }
                removedLines++;
            }
        }
        lines += removedLines;
        switch(removedLines)
        {
            case 1:
                score += 40;
                Mix_PlayChannel(-1, line, 0);
                break;
            case 2:
                score += 100;
                Mix_PlayChannel(-1, line, 0);
                break;
            case 3:
                score += 300;
                Mix_PlayChannel(-1, line, 0);
                break;
            case 4:
                score += 1200;
                Mix_PlayChannel(-1, line, 0);
                break;
            default:
                break;
        }
        tetromino.tetromino = nextTetromino[0].tetromino;
        tetromino.rotation = nextTetromino[0].rotation;
        tetromino.colour = nextTetromino[0].colour;
        tetromino.pos = {268, 10};
        for(int i = 0; i < 4; i++)
        {
            nextTetromino[i].tetromino = nextTetromino[i + 1].tetromino;
            nextTetromino[i].colour = nextTetromino[i + 1].colour;
        } 
        randomiseTetromino(nextTetromino[4]);
        holdLock = false;
    }
        
}
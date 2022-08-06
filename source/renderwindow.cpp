#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "renderwindow.h"
#include "sprite.h"

extern SDL_Renderer* gRenderer;

RenderWindow::RenderWindow(const char *p_title, int p_w, int p_h)
    : window(nullptr), renderer(nullptr), width(p_w), height(p_h)
{
    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        std::cout << "could not create window : " << SDL_GetError();
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    gRenderer = renderer;
    if (renderer == nullptr)
    {
        std::cout << "could not create renderer : " << SDL_GetError();
        
    }
    if(SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND))
    {
        std::cout << "could not set render draw blend mode :" << SDL_GetError();
    }
}
//deconstructor empty due to issues with it being triggered when setting a renderwindow to another renderwindow
RenderWindow::~RenderWindow()
{
    //cleanup();
}

SDL_Texture *RenderWindow::loadTexture(const char *filePath)
{
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, filePath);
    if(texture == NULL)
    {
        std::cout << "could not load texture at \" " << filePath << " \" " << IMG_GetError();
    }
    loadedTextures.push_back(texture);
    return texture;
}

SDL_Surface *RenderWindow::loadSurface(const char *filePath)
{
    SDL_Surface* surface = NULL;
    surface = IMG_Load(filePath);
    if(surface == NULL)
    {
        std::cout << "could not load surface at \" " << filePath << " \" " << IMG_GetError();
    }
    loadedSurfaces.push_back(surface);
    return surface;
}

void RenderWindow::clear()
{
    SDL_RenderClear(renderer);
}

void RenderWindow::cleanup()
{
    for(auto& t : loadedTextures)
    {
        if(t != nullptr)
        {
            SDL_DestroyTexture(t);
            t = nullptr;
        }
    }
    for(auto& s : loadedSurfaces)
    {
        if (s != nullptr)
        {
            SDL_FreeSurface(s);
            s = nullptr;
        }
    }
    if (window != nullptr)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    if (renderer != nullptr)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
}
void RenderWindow::render(SDL_Texture *texture)
{
    SDL_RenderCopy(renderer, texture, NULL, NULL);
}

void RenderWindow::render(SDL_Texture *texture, SDL_Rect textureRect)
{
    SDL_RenderCopy(renderer, texture, NULL, &textureRect);
}

void RenderWindow::render(SDL_Texture *texture, float p_x, float p_y)
{
    SDL_Rect rect;
    rect.x = p_x;
    rect.y = p_y;
    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void RenderWindow::render(SDL_Texture *texture, Vector2f pos)
{
    SDL_Rect rect;
    rect.x = pos.x;
    rect.y = pos.y;
    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void RenderWindow::render(SDL_Surface *surface)
{
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
}

void RenderWindow::render(SDL_Surface *surface, SDL_Rect surfaceRect)
{
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, &surfaceRect);
}

void RenderWindow::render(SDL_Surface *surface, float p_x, float p_y)
{
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {(int)p_x, (int)p_y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void RenderWindow::render(SDL_Surface *surface, Vector2f pos)
{
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {(int)pos.x, (int)pos.y, surface->w, surface->h};

    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

// void RenderWindow::render(Entity &entity)
// {
//     SDL_Rect srcRect = entity.getSrcRect();
//     SDL_Rect dstRect = entity.getDstRect();

//     SDL_Point centre = entity.getSprite().centre;

//     SDL_RenderCopyEx(renderer, entity.getTex(), &srcRect, &dstRect, entity.getRotation(), &centre, entity.getSprite().flip);
// }

void RenderWindow::render(Tetromino &tetromino)
{
    std::string piece = "";
    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            piece += tetromino.tetromino[tetrominoRotateIndex(tetromino, x, y)];
        }
    }
    //std::cout << piece;

    for (int i = 0; (size_t)i < piece.length(); i++)
    {
        if (piece[i] == 'X')
        {
            drawRect({(int)tetromino.x + tetromino.w * (i % 4), (int)tetromino.y + (int)(31 * floor(i/4)), tetromino.w + 1 , tetromino.h + 1 }, tetromino.colour);
        }
    };
}

void RenderWindow::drawLine(int x1, int y1, int x2, int y2, SDL_Colour lineColour)
{
    SDL_SetRenderDrawColor(renderer, lineColour.r, lineColour.g, lineColour.b, lineColour.a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void RenderWindow::drawRect(int x, int y, int w, int h, SDL_Colour rectColour)
{
    SDL_SetRenderDrawColor(renderer, rectColour.r, rectColour.g, rectColour.b, rectColour.a);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(renderer, &rect); 
}

void RenderWindow::drawRect(SDL_Rect rect, SDL_Colour rectColour)
{
    SDL_SetRenderDrawColor(renderer, rectColour.r, rectColour.g, rectColour.b, rectColour.a);
    SDL_RenderFillRect(renderer, &rect);
}

void RenderWindow::drawRectF(float x, float y, float w, float h, SDL_Colour rectColour)
{
    SDL_SetRenderDrawColor(renderer, rectColour.r, rectColour.g, rectColour.b, rectColour.a);
    SDL_FRect FRect = {x, y, w, h};
    SDL_RenderFillRectF(renderer, &FRect);
}

void RenderWindow::drawRectF(SDL_FRect FRect, SDL_Colour rectColour)
{
    SDL_SetRenderDrawColor(renderer, rectColour.r, rectColour.g, rectColour.b, rectColour.a);
    SDL_RenderFillRectF(renderer, &FRect);
}

void RenderWindow::setTitle(const char *title)
{
    SDL_SetWindowTitle(window, title);
}

void RenderWindow::setIcon(SDL_Surface *icon)
{
    SDL_SetWindowIcon(window, icon);
}

void RenderWindow::setIcon(const char* iconPath)
{
    SDL_Surface *icon = IMG_Load(iconPath);
    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);
}

void RenderWindow::setDrawColour(SDL_Colour colour)
{
    SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
}

void RenderWindow::createErrorBox(const char *title, const char *message)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message, window);
}

void RenderWindow::display()
{
    SDL_RenderPresent(renderer);
}

void RenderWindow::handleEvents(SDL_Event &e)
{
    if(e.type == SDL_WINDOWEVENT)
    {
        switch(e.window.event)
        {
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                width = e.window.data1;
                height = e.window.data2;
                viewport.w = width;
                viewport.h = height;
                break;
            default:
                break;
        }
    }
}
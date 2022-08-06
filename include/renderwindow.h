#pragma once
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#include "utils.h"
#include "sprite.h"
#include "entity.h"
#include "tetromino.h"

class RenderWindow
{
public:
    RenderWindow() :window(nullptr), renderer(nullptr) {}
    RenderWindow(const char *p_title, int p_w, int p_h);
    ~RenderWindow();

    SDL_Texture *loadTexture(const char *filePath);
    SDL_Surface *loadSurface(const char *filePath);

    void clear();
    void cleanup();

    void render(SDL_Texture *texture);
    void render(SDL_Texture *texture, SDL_Rect textureRect);
    void render(SDL_Texture *texture, float p_x, float p_y);
    void render(SDL_Texture *texture, Vector2f pos);

    void render(SDL_Surface *surface);
    void render(SDL_Surface *surface, SDL_Rect surfaceRect);
    void render(SDL_Surface *surface, float p_x, float p_y);
    void render(SDL_Surface *surface, Vector2f pos);

    void render(Entity& entity);
    void render(Tetromino& tetromino);

    void drawLine(int x1, int y1, int x2, int y2, SDL_Colour lineColour);
    void drawRect(int x, int y, int w, int h, SDL_Colour rectColour);
    void drawRect(SDL_Rect rect, SDL_Colour rectColour);
    void drawRectF(float x, float y, float w, float h, SDL_Colour rectColour);
    void drawRectF(SDL_FRect FRect, SDL_Colour rectColour);

    void setTitle(const char *title);
    void setIcon(SDL_Surface *icon);
    void setIcon(const char* iconPath);
    void setDrawColour(SDL_Colour colour);

    void createErrorBox(const char *title, const char *message);

    void display();
    void handleEvents(SDL_Event &e);

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    Vector2i getWH() const { return size; }
    void setWH(int w, int h) {width = w; height = h; viewport.w = w; viewport.h = h; SDL_SetWindowSize(window, width, height); }

    void setScale(float x, float y){SDL_RenderSetScale(renderer, x, y);};

private:
    SDL_Window *window;
    SDL_Renderer *renderer;

    std::vector<SDL_Texture *> loadedTextures = {};
    std::vector<SDL_Surface *> loadedSurfaces = {};

    SDL_Rect viewport = {0,0,0,0};

    union
    {
        struct
        {
            int width, height;
        };
        Vector2i size;
    };
    
};
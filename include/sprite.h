#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

extern SDL_Renderer *gRenderer;

struct Sprite
{
    Sprite()
    {

    }
    Sprite(const char* p_path) 
    {
        texture = IMG_LoadTexture(gRenderer, p_path);
        if (texture == nullptr)
        {
            std::cout << "could not load texture " << SDL_GetError() << "\n";
        }
        SDL_QueryTexture(texture, NULL, NULL, &srcW, &srcH);
        SDL_QueryTexture(texture, NULL, NULL, &dstW, &dstH);
        centreX = dstW / 2;
        centreY = dstH / 2;
    }
    Sprite(SDL_Texture* p_texture)
    {
        texture = p_texture;
        if (texture == nullptr)
        {
            std::cout << "could not load texture " << SDL_GetError() << "\n";
        }
        SDL_QueryTexture(texture, NULL, NULL, &srcW, &srcH);
        SDL_QueryTexture(texture, NULL, NULL, &dstW, &dstH);
        centreX = dstW / 2;
        centreY = dstH / 2;
    }
    union
    {
        struct
        {
            int dstX, dstY, dstW, dstH;
        };

        SDL_Rect dstRect = {0,0,0,0};
    };
    union
    {
        struct 
        {
            int srcX, srcY, srcW, srcH;
        };
        
        SDL_Rect srcRect = {0,0,0,0};
    };
    union 
    {
        struct 
        {
            Uint8 r, g, b, alpha;
        };
        
        SDL_Colour colourMod = {255,255,255,255};
    };
    union
    {
        struct
        {
            int centreX, centreY;
        };
        SDL_Point centre;
    };
    
    SDL_BlendMode blendmode = SDL_BLENDMODE_BLEND;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    SDL_Texture* texture = nullptr;
};
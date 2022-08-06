#pragma once
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "sprite.h"
#include "animation.h"

class Entity
{
public:
    Entity() {}
    Entity(Vector2f p_pos, Sprite p_sprite);
    Entity(Vector2f p_pos, SDL_Texture* texture);
    Entity(Vector2f p_pos, Vector2f p_scale, SDL_Texture *texture);
    Entity(Vector2f p_pos, Vector2f p_scale, SDL_Texture* texture, double p_rotation, SDL_RendererFlip p_flip);

    virtual ~Entity();
    virtual void free();

    Vector2f getPos() { return pos; }
    void setPos(Vector2f p_pos);

    Vector2f getScale() { return scale; }
    void setScale(Vector2f p_scale);

    double getRotation() { return rotation; }
    void setRotation(double p_rotation) { rotation = p_rotation; }

    void setBlendMode(SDL_BlendMode blend);
    SDL_BlendMode getBlendMode() { return sprite.blendmode; }

    Sprite getSprite() { return sprite; }
    void setSprite(Sprite p_sprite) { sprite = p_sprite; }

    SDL_Rect getSrcRect() { return sprite.srcRect; }
    void setSrcRect(SDL_Rect p_rect) { sprite.srcRect = p_rect; } 

    SDL_Rect getDstRect() { return sprite.dstRect; }
    void setDstRect(SDL_Rect p_rect) { sprite.dstRect = p_rect; }

    SDL_Texture* getTex() { return sprite.texture; }
    void setTex(SDL_Texture* p_tex) { sprite.texture = p_tex; }

    SDL_Rect getHitbox() { return globalHitbox; }
    void setHitbox(SDL_Rect p_hitbox); // set the hit box relative to the player

    SDL_RendererFlip getFlip() { return sprite.flip; }
    void setFlip(SDL_RendererFlip p_flip) { sprite.flip = p_flip; }

    SDL_Colour getRGBAMod() { return sprite.colourMod; }
    void modRGBA(SDL_Colour rgba);

    void modR(Uint8 p_r);
    void modG(Uint8 p_g);
    void modB(Uint8 p_b);
    void modA(Uint8 p_a);

    Uint8 getRMod();
    Uint8 getGMod();
    Uint8 getBMod();
    Uint8 getAMod();

   // void loadAnimationSet(const char *filePath) { animationCycle = loadAnimationFile(filePath); }

    void play(std::string key, float deltaTime);

protected:
    Vector2f pos = Vector2f(0.0f,0.0f);
    Vector2f scale = Vector2f(1.0f, 1.0f);
    double rotation = 0.0; // rotation in degrees

    SDL_Rect relativeHitbox = {0,0,0,0}; // hitbox with positions relative to entity
    SDL_Rect globalHitbox = {0,0,0,0}; //hitbox with global positions

    std::map<std::string, animationSet> animationCycle = {};

    Sprite sprite;
};


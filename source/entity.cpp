#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "utils.h"
#include "sprite.h"
#include "entity.h"

Entity::Entity(Vector2f p_pos, Sprite p_sprite)
    :pos(p_pos), sprite(p_sprite)
{
    sprite.dstX = pos.x;
    sprite.dstY = pos.y;
    SDL_SetTextureBlendMode(sprite.texture, sprite.blendmode);
}

Entity::Entity(Vector2f p_pos, SDL_Texture *texture)
    :pos(p_pos)
{
    sprite = Sprite(texture);
    sprite.dstX = pos.x;
    sprite.dstY = pos.y;
    SDL_SetTextureBlendMode(sprite.texture, sprite.blendmode);
}

Entity::Entity(Vector2f p_pos, Vector2f p_scale, SDL_Texture *texture)
    :pos(p_pos), scale(p_scale)
{
    sprite = Sprite(texture);
    sprite.dstX = pos.x;
    sprite.dstY = pos.y;
    sprite.dstW = sprite.srcW * scale.x;
    sprite.dstH = sprite.srcH * scale.y;
    sprite.centreX = sprite.dstW / 2;
    sprite.centreY = sprite.dstH / 2;
    SDL_SetTextureBlendMode(sprite.texture, sprite.blendmode);
}

Entity::Entity(Vector2f p_pos, Vector2f p_scale, SDL_Texture *texture, double p_rotation, SDL_RendererFlip p_flip)
    : pos(p_pos), scale(p_scale), rotation(p_rotation)

{
    sprite = Sprite(texture);
    sprite.dstX = pos.x;
    sprite.dstY = pos.y;
    sprite.dstW = sprite.srcW * scale.x;
    sprite.dstH = sprite.srcH * scale.y;
    sprite.flip = p_flip;
    sprite.centreX = sprite.dstW / 2;
    sprite.centreY = sprite.dstH / 2;
    SDL_SetTextureBlendMode(sprite.texture, sprite.blendmode);
}

Entity::~Entity()
{
    free();
}

void Entity::free()
{
    pos = Vector2f(0.0f, 0.0f);
    sprite.srcRect = {0, 0, 0, 0};
    sprite.dstRect = {0, 0, 0, 0};
}

void Entity::setPos(Vector2f p_pos) 
{
    pos = p_pos; 
    sprite.dstX = pos.x;
    sprite.dstY = pos.y;
    globalHitbox.x = relativeHitbox.x + pos.x;
    globalHitbox.y = relativeHitbox.y + pos.y;
}

void Entity::setHitbox(SDL_Rect p_hitbox)
{
    relativeHitbox = p_hitbox;
    globalHitbox = relativeHitbox;
    globalHitbox.x = relativeHitbox.x + pos.x;
    globalHitbox.y = relativeHitbox.y + pos.y;
}

void Entity::setScale(Vector2f p_scale) 
{
    scale = p_scale; 
    sprite.dstW = sprite.srcW * scale.x;
    sprite.dstH = sprite.srcH * scale.y;
    sprite.centreX = sprite.dstW / 2;
    sprite.centreY = sprite.dstH / 2;
    globalHitbox.x = relativeHitbox.x * scale.x;
    globalHitbox.y = relativeHitbox.y * scale.y;
}

void Entity::setBlendMode(SDL_BlendMode blend)
{ 
    sprite.blendmode = blend; 
    SDL_SetTextureBlendMode(sprite.texture, sprite.blendmode);
}

void Entity::modRGBA(SDL_Colour rgba)
{
    sprite.r = rgba.r;
    sprite.g = rgba.g;
    sprite.b = rgba.b;
    sprite.alpha = rgba.a;
    SDL_SetTextureColorMod(sprite.texture, sprite.r, sprite.g, sprite.b);
    SDL_SetTextureAlphaMod(sprite.texture, sprite.alpha);
}

void Entity::modR(Uint8 p_r)
{
    sprite.r = p_r;
    SDL_SetTextureColorMod(sprite.texture, sprite.r, sprite.g, sprite.b);
}

void Entity::modG(Uint8 p_g)
{
    sprite.g = p_g;
    SDL_SetTextureColorMod(sprite.texture, sprite.r, sprite.g, sprite.b);
}

void Entity::modB(Uint8 p_b)
{
    sprite.b = p_b;
    SDL_SetTextureColorMod(sprite.texture, sprite.r, sprite.g, sprite.b);
}

void Entity::modA(Uint8 p_a)
{
    sprite.alpha = p_a;
    SDL_SetTextureAlphaMod(sprite.texture, sprite.alpha);
}

Uint8 Entity::getRMod()
{
    return sprite.r;
}

Uint8 Entity::getGMod()
{
    return sprite.g;
}

Uint8 Entity::getBMod()
{
    return sprite.b;
}

Uint8 Entity::getAMod()
{
    return sprite.alpha;
}

void Entity::play(std::string key, float deltaTime)
{
    animationCycle[key].delta += 10 * deltaTime;
    if (animationCycle[key].delta >= animationCycle[key].animationFrames[animationCycle[key].currentFrame].duration * deltaTime / animationCycle[key].speed)
    {
        animationCycle[key].currentFrame++;
        if (animationCycle[key].currentFrame >= animationCycle[key].animationFrames.size())
            animationCycle[key].currentFrame = 0;

        animationCycle[key].delta = 0;

        setSrcRect(animationCycle[key].animationFrames[animationCycle[key].currentFrame].frame);
    }
}

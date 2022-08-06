//made for my great aunt
#include <iostream>
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include "game.h"
#include "renderwindow.h"
#include "utils.h"

bool init()
{    
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        std::cout << "could not init sdl2: " << SDL_GetError() << "\n Press any enter to exit";
        std::cin.get();
        return 0;
    }
#ifndef __EMSCRIPTEN__
    if(!IMG_Init(IMG_INIT_PNG))
    {
        std::cout << "could not init sdl2_image: " << IMG_GetError() << "\n Press any enter to exit";
        std::cin.get();
        return 0;
    }
#endif
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cout << "sdl mixer has dfailed lol: " << Mix_GetError() << "\n";
        std::cin.get();
        return 0;
    }

    return true;
}

bool SDLinit = init();

SDL_Renderer* gRenderer;

Game game;

/* em++ -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_SDL_MIXER=2 -sMAX_WEBGL_VERSION=2 -sMIN_WEBGL_VERSION=2 -std=c++20 -Iinclude -Ires source/main.cpp source/tetromino.cpp source/game.cpp source/renderwindow.cpp source/entity.cpp --preload-file .\res -o b.html --use-preload-plugins -mnontrapping-fptoint -s ALLOW_MEMORY_GROWTH=1
 */
#ifdef __EMSCRIPTEN__
void gameLoop()
{
    game.run();
}
#endif
int main(int argc, char* argv[])
{
    //srand(time(NULL));
    game.init();
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(gameLoop, 0, 1);
#else
    game.run();
#endif
    game.cleanup();
    
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
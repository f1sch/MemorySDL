#ifndef MEMORYSDL_GAMECONTEXT_H
#define MEMORYSDL_GAMECONTEXT_H
#include "AssetManager.h"
#include "GridLayout.h"
#include "SoundSystem.h"

#include "SDL3/SDL_render.h"

struct GameContext
{
    SDL_Renderer *renderer;
    AssetManager *assetManager;
    GridLayout *grid;
    SoundSystem *soundSystem;

    int windowWidth;
    int windowHeight;
    int texWidth;
    int texHeight;

    GameContext(SDL_Renderer* r, AssetManager *am, GridLayout *gl, SoundSystem *ss,
        const int wWidth, const int wHeight, const int tw, const int th)
        : renderer(r), assetManager(am), grid(gl), soundSystem(ss), windowWidth(wWidth),
          windowHeight(wHeight), texWidth(tw),
          texHeight(th)
    {}
};

#endif //MEMORYSDL_GAMECONTEXT_H
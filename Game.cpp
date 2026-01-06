#include "Game.h"
#include "AssetManager.h"

#include <memory>

Game::Game(SDL_Window* window, SDL_Renderer* renderer, int width, int height)
    : m_window(window), m_renderer(renderer),
    m_windowWidth(width), m_windowHeight(height)
{
}

Game::~Game()
{
}

int Game::Init()
{
    m_assetManager = std::make_unique<AssetManager>(m_renderer);
    m_assetManager->LoadTextures();

	return 0;
}

int Game::Run()
{
    SDL_FRect dst_rect;
    const Uint64 now = SDL_GetTicks();

    // we'll have some textures move around over a few seconds.
    const float direction = ((now % 2000) >= 1000) ? 1.0f : -1.0f;
    const float scale = ((float)(((int)(now % 1000)) - 500) / 500.0f) * direction;

    // as you can see from this, rendering draws over whatever was drawn before it.
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  // black, full alpha
    SDL_RenderClear(m_renderer);  // start with a blank canvas.

    // Just draw the static texture a few times. You can think of it like a
    // stamp, there isn't a limit to the number of times you can draw with it.

    // top left
    //dst_rect.x = (100.0f * scale);
    //dst_rect.y = 0.0f;
    //dst_rect.w = (float)texture_width;
    //dst_rect.h = (float)texture_height;
    //SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

    int margin = 20;
    for (SDL_Texture* t : m_assetManager->GetTextures())
    {
        dst_rect.x = ((float)(m_windowWidth - t->w)) / 2.0f - margin;
        dst_rect.y = ((float)(m_windowHeight - t->h)) / 2.0f;
        dst_rect.w = (float)t->w;
        dst_rect.h = (float)t->h;
        SDL_RenderTexture(m_renderer, t, NULL, &dst_rect);

        dst_rect.x = ((float)(m_windowWidth - t->w)) / 2.0f + margin;
        dst_rect.y = ((float)(m_windowHeight - t->h)) / 2.0f;
        dst_rect.w = (float)t->w;
        dst_rect.h = (float)t->h;
        SDL_RenderTexture(m_renderer, t, NULL, &dst_rect);
        
        margin += 20;
    }

    // bottom right.
    //dst_rect.x = ((float)(WINDOW_WIDTH - texture_width)) - (100.0f * scale);
    //dst_rect.y = (float)(WINDOW_HEIGHT - texture_height);
    //dst_rect.w = (float)texture_width;
    //dst_rect.h = (float)texture_height;
    //SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

    SDL_RenderPresent(m_renderer);  // put it all on the screen!

    return 0;
}

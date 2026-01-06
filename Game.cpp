#include "Game.h"
#include "AssetManager.h"

#include <memory>

constexpr auto TEX_WIDTH = 32;
constexpr auto TEX_HEIGHT = 32;

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
    m_assetManager->LoadTexture("Back", "assets/Back.png");
    m_assetManager->LoadTexture("Skull", "assets/Skull.png");
    
    // TODO: irgendwo gehen die Koordinaten kaputt, wenn die Größe
    // des Fensters geändert wird

    auto* tex = m_assetManager->GetTexture("Back");
    m_cards.push_back(Card{
        .frontKey = "Skull",
        .dst = SDL_FRect{static_cast<float>(m_windowWidth / 2), static_cast<float>(m_windowHeight/2), 
            static_cast<float>(TEX_WIDTH), static_cast<float>(TEX_HEIGHT)},
        .id = 0
    });
    m_cards.push_back(Card{
        .frontKey = "Skull",
        .dst = SDL_FRect{100, 100, static_cast<float>(TEX_WIDTH), static_cast<float>(TEX_HEIGHT)},
        .id = 1
    });

	return 0;
}

int Game::Run()
{
    SDL_FRect dst_rect{};
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
 
    SDL_GetWindowSize(m_window, &m_windowWidth, &m_windowHeight);

    int margin = 20;
    for (auto& card : m_cards)
    {
        dst_rect.x = ((float)(m_windowWidth - card.dst.w)) / 2.0f - margin;
        dst_rect.y = ((float)(m_windowHeight - card.dst.h)) / 2.0f;
        dst_rect.w = (float)card.dst.w;
        dst_rect.h = (float)card.dst.h;
        card.dst = dst_rect;
        SDL_RenderTexture(m_renderer, m_assetManager->GetTexture("Back"), NULL, &dst_rect);
        
        margin *= -1;
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

void Game::HitTest(float x, float y)
{
    // TODO: Koordinaten müssen noch in Screen Coordinates umgewandelt werden
    SDL_Point p = { static_cast<int>(x), static_cast<int>(y) };
    bool hit = false;
    for (auto& card : m_cards)
    {
        SDL_Rect rect = { 
            static_cast<int>(card.dst.x), 
            static_cast<int>(card.dst.y), 
            static_cast<int>(card.dst.w), 
            static_cast<int>(card.dst.h) 
        };
        hit = SDL_PointInRect(&p, &rect);
        if (hit)
        {
            SDL_Log("Point is in rect!");
        }
        auto contains = card.contains(x, y);
        if (contains)
        {
            SDL_Log("Rect contains point!");
        }
    }
}

#include "pch.h"

#include "AssetManager.h"
#include "Card.h"
#include "Game.h"
#include "GridLayout.h"
#include "SoundSystem.h"

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>

#include <memory>
#include <string>
#include <vector>

constexpr auto TEX_WIDTH = 32 * 3;
constexpr auto TEX_HEIGHT = 32 * 3;

Game::Game(SDL_Window* window, SDL_Renderer* renderer, int width, int height)
    : 
    m_window(window), m_renderer(renderer), 
    m_windowWidth(width), m_windowHeight(height), 
    m_cardsSelected(CardSelected::NoCard)
{
}

Game::~Game()
{
}

int Game::Init()
{
    std::vector<std::string> frontCards = { "Skull", "Coffin", "Candle", "Dagger", "Bat", "Door" };
    
    m_assetManager = std::make_unique<AssetManager>(m_renderer);
    m_assetManager->LoadTexture("Back");
    
    for (auto& s : frontCards)
    {
        m_assetManager->LoadTexture(s);
    }
    
    m_grid = std::make_unique<GridLayout>(3, 4);
    m_grid->BuildDeck(frontCards);
    m_grid->InitGrid(m_windowWidth, m_windowHeight, TEX_WIDTH, TEX_HEIGHT);

    m_soundSystem = std::make_unique<SoundSystem>();
    m_soundSystem->Init();

	return 0;
}

int Game::Update()
{
    //const Uint64 now = SDL_GetTicks();

    // we'll have some textures move around over a few seconds.
    //const float direction = ((now % 2000) >= 1000) ? 1.0f : -1.0f;
    //const float scale = ((float)(((int)(now % 1000)) - 500) / 500.0f) * direction;

    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_renderer);
    
    if (m_cardsSelected == CardSelected::TwoCards) {
        if (SDL_GetTicks() >= m_resolveCardsAtMs) {
            Card& a = m_grid->m_cards.at(m_firstCardIdx);
            Card& b = m_grid->m_cards.at(m_secondCardIdx);

            if (a.pairId == b.pairId) {
                a.state = CardState::Matched;
                b.state = CardState::Matched;
            } else {
                a.state = CardState::FaceDown;
                b.state = CardState::FaceDown;
            }

            m_firstCardIdx = -1;
            m_secondCardIdx = -1;
            m_cardsSelected = CardSelected::NoCard;
        }
    }

    // Iterate over GridLayout and render
    const int size = static_cast<int>(m_grid->GetSize());
    for (int i = 0; i < size; ++i)
    {
        SDL_Texture* tex = m_assetManager->GetTexture("Back");
        auto& card = m_grid->m_cards.at(i);
        auto& rect = m_grid->m_grid.at(i);

        if (card.state != CardState::FaceDown) {
            tex = m_assetManager->GetTexture(card.frontKey);
        }

        SDL_RenderTexture(m_renderer, tex, NULL, &rect);
    }

    SDL_RenderPresent(m_renderer);

    m_soundSystem->LoopSound(SoundSystem::SoundId::Background);

    return 0;
}

void Game::Resize()
{
    SDL_GetWindowSizeInPixels(m_window, &m_windowWidth, &m_windowHeight);
    m_grid->InitGrid(m_windowWidth, m_windowHeight, TEX_WIDTH, TEX_HEIGHT);
}

void Game::HitTest(float x, float y)
{
    // HitTests are locked if two Cards are currently faceUp
    if (m_cardsSelected == CardSelected::TwoCards)
        return;

    SDL_Point p = { static_cast<int>(x), static_cast<int>(y) };
    
    const int size = static_cast<int>(m_grid->GetSize());
    for (int i = 0; i < size; ++i)
    {
        Card& card = m_grid->m_cards.at(i);
        
        // Only HitTest Cards that are facedown
        if (card.state != CardState::FaceDown)
            continue;

        // Check if Grid was hit
        auto& gridRect = m_grid->m_grid.at(i);
        SDL_Rect rc{ 
            static_cast<int>(gridRect.x), static_cast<int>(gridRect.y), 
            static_cast<int>(gridRect.w), static_cast<int>(gridRect.h)
        };
        if (!SDL_PointInRect(&p, &rc))
            continue;

        // Do nothing if the same Card is clicked repeatedly
        if (m_cardsSelected == CardSelected::OneCard && m_firstCardIdx == i)
            return;

        // TODO: background music gets cut and cardFLip.wav is not played
        m_soundSystem->PlaySound(SoundSystem::SoundId::CardFlip);

        card.state = CardState::FaceUp;
        
        // Current Card is the first Card that was clicked
        if (m_cardsSelected == CardSelected::NoCard) {
            m_firstCardIdx = static_cast<int>(i);
            m_cardsSelected = CardSelected::OneCard;
            return;
        }

        m_secondCardIdx = static_cast<int>(i);
        m_cardsSelected = CardSelected::TwoCards;
        m_resolveCardsAtMs = SDL_GetTicks() + m_revealDelayMs;
        return;
    }
}
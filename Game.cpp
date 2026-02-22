#include "Game.h"
#include "AssetManager.h"
#include "GridLayout.h"

#include <memory>

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

	return 0;
}

int Game::Update()
{
    const Uint64 now = SDL_GetTicks();

    // we'll have some textures move around over a few seconds.
    const float direction = ((now % 2000) >= 1000) ? 1.0f : -1.0f;
    const float scale = ((float)(((int)(now % 1000)) - 500) / 500.0f) * direction;

    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_renderer);

    // top left
    //dst_rect.x = (100.0f * scale);
    //dst_rect.y = 0.0f;
    //dst_rect.w = (float)texture_width;
    //dst_rect.h = (float)texture_height;
    //SDL_RenderTexture(renderer, texture, NULL, &dst_rect);
    
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
    for (size_t i{}; i < m_grid->GetSize(); ++i)
    {
        SDL_Texture* tex = m_assetManager->GetTexture("Back");

        if (m_grid->m_cards.at(i).state != CardState::FaceDown) {
            tex = m_assetManager->GetTexture(m_grid->m_cards.at(i).frontKey);
        }

        SDL_RenderTexture(m_renderer, tex, NULL, &m_grid->m_grid.at(i));
    }
    // bottom right.
    //dst_rect.x = ((float)(WINDOW_WIDTH - texture_width)) - (100.0f * scale);
    //dst_rect.y = (float)(WINDOW_HEIGHT - texture_height);
    //dst_rect.w = (float)texture_width;
    //dst_rect.h = (float)texture_height;
    //SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

    SDL_RenderPresent(m_renderer);

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
    
    for (size_t i{}; i < m_grid->GetSize(); ++i)
    {
        Card& card = m_grid->m_cards.at(i);
        
        // Only HitTest Cards that are facedown
        if (card.state != CardState::FaceDown)
            continue;

        // Check if Grid was hit
        // find() key (card.uniqueId) und dann hole die id aus map -> rect
        //auto id = m_grid->m_grid.find(card.uniqueId);
        SDL_Point p{ x,y };
        auto& gridRect = m_grid->m_grid.at(i);
        SDL_Rect rc{ gridRect.x, gridRect.y, gridRect.w, gridRect.h};
        if (!SDL_PointInRect(&p, &rc))
            continue;

        // Do nothing if the same Card is clicked repeatedly
        if (m_cardsSelected == CardSelected::OneCard && m_firstCardIdx == i)
            return;

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

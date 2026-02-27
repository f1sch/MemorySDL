//#include "pch.h"

#include "AssetManager.h"
#include "Card.h"
#include "Game.h"
#include "GridLayout.h"
#include "SoundSystem.h"

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>

#include <memory>
#include <string>
#include <vector>

constexpr auto TEX_WIDTH = 32 * 3;
constexpr auto TEX_HEIGHT = 32 * 3;

Game::Game(SDL_Window* window, SDL_Renderer* renderer, const int width, const int height)
    :
    m_window(window), m_renderer(renderer),
    m_windowWidth(width), m_windowHeight(height),
    m_gameState(GameState::Paused),
    m_cardsSelected(CardSelected::NoCard)
{
}

Game::~Game()
{
    ShutdownGame();
}

int Game::Init()
{
    const std::vector<std::string> frontCards = { "Skull", "Coffin", "Candle", "Dagger", "Bat", "Door" };
    
    m_assetManager = std::make_unique<AssetManager>(m_renderer);
    m_assetManager->LoadTexture("Back");
    
    // TESTING:
    constexpr int numOfCards = 6; // set to number of cards that will be rendered
    std::vector<std::string> testingCards{};
    for (int i{}; i < numOfCards; ++i)
    {
        m_assetManager->LoadTexture(frontCards[i]);
        testingCards.push_back(frontCards[i]);
    }
    
    m_assetManager->LoadTexture("PlayButton");
    m_assetManager->LoadTexture("QuitButton");

    m_grid = std::make_unique<GridLayout>(numOfCards/2, 4);
    m_grid->BuildDeck(testingCards);
    m_grid->InitGrid(m_windowWidth, m_windowHeight, TEX_WIDTH, TEX_HEIGHT);

    m_soundSystem = std::make_unique<SoundSystem>();
    m_soundSystem->Init();

    // UI
    m_playButtonRect.x = static_cast<float>(m_windowWidth) * 0.15f;
    m_playButtonRect.y = static_cast<float>(m_windowHeight) * 0.75f;
    m_playButtonRect.w = TEX_WIDTH;
    m_playButtonRect.h = TEX_HEIGHT;

    m_quitButtonRect.x = static_cast<float>(m_windowWidth) * 0.75f;
    m_quitButtonRect.y = static_cast<float>(m_windowHeight) * 0.75f;
    m_quitButtonRect.w = TEX_WIDTH;
    m_quitButtonRect.h = TEX_HEIGHT;

    return 0;
}

void Game::Start()
{
    m_gameState = GameState::Starting;
}

void Game::Run()
{
    m_gameState = GameState::Running;
}

void Game::ShutdownGame() const
{
    m_soundSystem->ShutdownSound();
}

int Game::Update()
{
    //const Uint64 now = SDL_GetTicks();
    // we'll have some textures move around over a few seconds.
    //const float direction = ((now % 2000) >= 1000) ? 1.0f : -1.0f;
    //const float scale = ((float)(((int)(now % 1000)) - 500) / 500.0f) * direction;

    int result = 0;
    switch (m_gameState)
    {
    case Game::GameState::Running:
        UpdateGameplay();
        Render();
        break;
    case Game::GameState::Ended:
        UpdateEndScreen();
        result = -1;
        break;
    case Game::GameState::Paused:
        break;
    case Game::GameState::Starting:
        UpdateStartScreen();
        break;
    default:
        break;
    }

    m_soundSystem->LoopMusic(SoundSystem::SoundId::Background);

    return result;
}

void Game::UpdateGameplay()
{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_renderer);

    if (m_cardsSelected == CardSelected::TwoCards)
    {
        if (SDL_GetTicks() >= m_resolveCardsAtMs)
        {
            Card& a = m_grid->m_cards.at(m_firstCardIdx);
            Card& b = m_grid->m_cards.at(m_secondCardIdx);

            if (a.pairId == b.pairId)
            {
                a.state = CardState::Matched;
                b.state = CardState::Matched;
                m_numOfCardsMatched += 2;
            }
            else
            {
                a.state = CardState::FaceDown;
                b.state = CardState::FaceDown;
            }

            m_firstCardIdx = -1;
            m_secondCardIdx = -1;
            m_cardsSelected = CardSelected::NoCard;

            if (m_numOfCardsMatched == m_grid->GetSize())
                m_gameState = GameState::Ended;
        }
    }
}

void Game::UpdateEndScreen() const
{
    // Draw End screen
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_renderer);
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    // Ending title
    SDL_SetRenderScale(m_renderer, 5.f, 5.f);
    const auto text = "GAME ENDED!";
    const float middle = ((static_cast<float>(m_windowWidth) / 2.0f) / 5.f) - (static_cast<float>(SDL_strlen(text)) * 5.f) / 1.25f;
    SDL_RenderDebugText(m_renderer, middle, middle, text);
    SDL_SetRenderScale(m_renderer, 1.0f, 1.0f);

    // PlayButton
    SDL_Texture* play = m_assetManager->GetTexture("PlayButton");
    SDL_RenderTexture(m_renderer, play, nullptr, &m_playButtonRect);

    // QuitButton
    SDL_Texture* quit = m_assetManager->GetTexture("QuitButton");
    SDL_RenderTexture(m_renderer, quit, nullptr, &m_quitButtonRect);

    SDL_RenderPresent(m_renderer);
}

void Game::UpdateStartScreen() const
{
    // Draw Start screen
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_renderer);
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    // Starting title
    SDL_SetRenderScale(m_renderer, 4.f, 4.f);
    const auto text = "MEMORIA HORRIFICA";
    const float middle = ((static_cast<float>(m_windowWidth) / 2.0f) / 5.f) - (static_cast<float>(SDL_strlen(text)) * 5.f) / 1.55f;
    SDL_RenderDebugText(m_renderer, middle, 25.0f, text);
    SDL_SetRenderScale(m_renderer, 1.0f, 1.0f);

    // PlayButton
    SDL_Texture* play = m_assetManager->GetTexture("PlayButton");
    SDL_RenderTexture(m_renderer, play, nullptr, &m_playButtonRect);

    // QuitButton
    SDL_Texture* quit = m_assetManager->GetTexture("QuitButton");
    SDL_RenderTexture(m_renderer, quit, nullptr, &m_quitButtonRect);

    SDL_RenderPresent(m_renderer);
}

void Game::Render() const
{
    // Iterate over GridLayout and render
    const int size = static_cast<int>(m_grid->GetSize());
    for (int i = 0; i < size; ++i)
    {
        SDL_Texture* tex = m_assetManager->GetTexture("Back");
        auto& card = m_grid->m_cards.at(i);
        auto& rect = m_grid->m_grid.at(i);

        if (card.state != CardState::FaceDown)
        {
            tex = m_assetManager->GetTexture(card.frontKey);
        }

        SDL_RenderTexture(m_renderer, tex, nullptr, &rect);
    }

    SDL_RenderPresent(m_renderer);
}

void Game::Resize()
{
    SDL_GetWindowSizeInPixels(m_window, &m_windowWidth, &m_windowHeight);
    m_grid->InitGrid(m_windowWidth, m_windowHeight, TEX_WIDTH, TEX_HEIGHT);
}

// NOTE: return 0 if game loop should continue
int Game::HitTest(const float x, const float y)
{
    // HitTests are locked if two Cards are currently faceUp
    if (m_cardsSelected == CardSelected::TwoCards)
        return 0;

    const SDL_FPoint p = { x,y };
    if (m_gameState == GameState::Starting)
    {
        if (SDL_PointInRectFloat(&p, &m_playButtonRect))
        {
            Run();
            return 0;
        }
        if (SDL_PointInRectFloat(&p, &m_quitButtonRect))
            return -1;
    }

    // Check UI Buttons
    if (m_gameState == GameState::Ended)
    {
        if (SDL_PointInRectFloat(&p, &m_playButtonRect))
        {
            // Reset Game
            return 1;
        }
        if (SDL_PointInRectFloat(&p, &m_quitButtonRect))
        {
            // Quit Game
            return -1;
        }
        return 0;
    }

    const int size = static_cast<int>(m_grid->GetSize());
    for (int i = 0; i < size; ++i)
    {
        Card& card = m_grid->m_cards.at(i);
        
        // Only HitTest Cards that are face down
        if (card.state != CardState::FaceDown)
            continue;

        // Check if Grid was hit
        const auto& [gx, gy, gw, gh] = m_grid->m_grid.at(i);
        SDL_FRect rc{ gx, gy, gw, gh};
        if (!SDL_PointInRectFloat(&p, &rc))
            continue;

        // Do nothing if the same Card is clicked repeatedly
        if (m_cardsSelected == CardSelected::OneCard && m_firstCardIdx == i)
            return 0;

        m_soundSystem->PlaySfxSound(SoundSystem::SoundId::CardFlip);

        card.state = CardState::FaceUp;
        
        // Current Card is the first Card that was clicked
        if (m_cardsSelected == CardSelected::NoCard) 
        {
            m_firstCardIdx = i;
            m_cardsSelected = CardSelected::OneCard;
            return 0;
        }

        m_secondCardIdx = i;
        m_cardsSelected = CardSelected::TwoCards;
        m_resolveCardsAtMs = SDL_GetTicks() + m_revealDelayMs;
    }
    return 0;
}
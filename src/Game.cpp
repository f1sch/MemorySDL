//#include "pch.h"

#include "AssetManager.h"
#include "Card.h"
#include "Game.h"
#include "GridLayout.h"
#include "SceneManager.h"
#include "StartScene.h"
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

constexpr auto MAX_ATTEMPTS = 5;

Game::Game(SDL_Window* window, SDL_Renderer* renderer, const int width, const int height)
    : m_window(window), m_renderer(renderer),
      m_windowWidth(width), m_windowHeight(height), m_gameContext(nullptr, nullptr, 0, 0, 0, 0),
      m_gameState(GameState::Paused),
      m_cardsSelected(CardSelected::NoCard), m_attempts(MAX_ATTEMPTS)
{
}

Game::~Game()
{
    ShutdownGame();
}

int Game::Init()
{
    const std::vector<std::string> frontCards = { "Card_Skull", "Card_Coffin", "Card_Candle", "Card_Dagger", "Card_Bat", "Card_Door" };
    
    m_assetManager = std::make_unique<AssetManager>(m_renderer);
    m_assetManager->LoadTexture("Card_Back");
    
    // TESTING:
    constexpr int numOfCards = 6; // set to number of cards that will be rendered
    std::vector<std::string> testingCards{};
    for (int i{}; i < numOfCards; ++i)
    {
        m_assetManager->LoadTexture(frontCards[i]);
        testingCards.push_back(frontCards[i]);
    }
    const std::vector<std::string> uiTextures = { "UI_PlayButton", "UI_QuitButton", "UI_Heart"};
    for (const auto& tex : uiTextures)
    {
        m_assetManager->LoadTexture(tex);
    }

    m_grid = std::make_unique<GridLayout>(numOfCards/2, 4);
    m_grid->BuildDeck(testingCards);
    m_grid->ShuffleDeck();
    m_grid->InitGrid(m_windowWidth, m_windowHeight, TEX_WIDTH, TEX_HEIGHT);

    m_soundSystem = std::make_unique<SoundSystem>();
    m_soundSystem->Init();
    // only load on mouse input to start
#ifdef __EMSCRIPTEN__
#else
#endif

    // UI
    // NOTE: moved to StartScene::StartScene()
    //m_uiPlayButtonRect.x = static_cast<float>(m_windowWidth) * 0.15f;
    //m_uiPlayButtonRect.y = static_cast<float>(m_windowHeight) * 0.75f;
    //m_uiPlayButtonRect.w = static_cast<float>(TEX_WIDTH);
    //m_uiPlayButtonRect.h = static_cast<float>(TEX_HEIGHT);
    // NOTE: moved to StartScene::StartScene()
    // m_uiQuitButtonRect.x = static_cast<float>(m_windowWidth) * 0.75f;
    // m_uiQuitButtonRect.y = static_cast<float>(m_windowHeight) * 0.75f;
    // m_uiQuitButtonRect.w = static_cast<float>(TEX_WIDTH);
    // m_uiQuitButtonRect.h = static_cast<float>(TEX_HEIGHT);
    // TODO: move to GameScene::GameScene()
    constexpr auto size = static_cast<size_t>(MAX_ATTEMPTS);
    m_uiHeartRects.resize(size);
    for (size_t i = 0; i < size; ++i) {
        m_uiHeartRects[i].x = (static_cast<float>(m_windowWidth) * 0.5f + static_cast<float>(TEX_WIDTH * i) - static_cast<float>(TEX_WIDTH * m_attempts)/2);
        m_uiHeartRects[i].y = static_cast<float>(m_windowHeight) * 0.001f;
        m_uiHeartRects[i].w = static_cast<float>(TEX_WIDTH);
        m_uiHeartRects[i].h = static_cast<float>(TEX_HEIGHT);
    }

    m_gameContext.renderer = m_renderer;
    m_gameContext.assetManager = m_assetManager.get();
    m_gameContext.windowWidth = m_windowWidth;
    m_gameContext.windowHeight = m_windowHeight;
    m_gameContext.texWidth = TEX_WIDTH;
    m_gameContext.texHeight = TEX_HEIGHT;

    m_sceneManager = std::make_unique<SceneManager>();
    m_sceneManager->ChangeScene(std::make_unique<StartScene>(
        *m_sceneManager, m_gameContext)
    );

    return 0;
}

void Game::Start()
{
    m_gameState = GameState::Starting;
}

void Game::Restart()
{
    // shuffle deck
    m_grid->ShuffleDeck();
    // reset card states to FaceDown
    m_grid->ResetCardStates();
    // reset attempts
    m_attempts = MAX_ATTEMPTS;
    // reset state
    m_numOfCardsMatched = 0;
    m_cardsSelected = CardSelected::NoCard;
    m_firstCardIdx = -1;
    m_secondCardIdx = -1;
    // change scene
}

void Game::Run()
{
    m_gameState = GameState::Running;
}

void Game::ShutdownGame() const
{
    m_soundSystem->ShutdownSound();
}

// TODO: Add Scene management
int Game::Update()
{
    const Uint64 now = SDL_GetTicks();
    // we'll have some textures move around over a few seconds.
    //const float direction = ((now % 2000) >= 1000) ? 1.0f : -1.0f;
    //const float scale = ((float)(((int)(now % 1000)) - 500) / 500.0f) * direction;
    // TODO: 1. check return value (ends the game loop in main)
    int result = 0;
    // switch (m_gameState)
    // {
    // case GameState::Running:
    //     UpdateGameplay();
    //     Render();
    //     break;
    // case GameState::Ended:
    //     UpdateEndScreen();
    //     result = -1;
    //     break;
    // case GameState::Paused:
    //     break;
    // case GameState::Starting:
    //     UpdateStartScreen();
    //     break;
    // default:
    //     break;
    // }

    UpdateGameplay();
    m_sceneManager->Update(static_cast<float>(now));
    m_sceneManager->Render(m_renderer);

    //m_soundSystem->PlayMusic(SoundSystem::SoundId::Background);
#ifdef __EMSCRIPTEN__
    if (m_soundSystem->IsMusicStarted())
        m_soundSystem->Update();
#else
    m_soundSystem->Update();
#endif
    return result;
}

// NOTE: irrelevant with SceneManager
Game::GameCommand Game::OnMouseDown(const float x, const float y)
{
#ifdef __EMSCRIPTEN__
    if (!m_soundSystem->IsMusicStarted()) {
        m_soundSystem->SetMusicStarted(true);
        m_soundSystem->Update();
    }
#endif
    return HitTest(x,y);
}

void Game::UpdateGameplay()
{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_renderer);

    if (m_attempts < 1)
    {
        m_gameState = GameState::Ended; // NOTE: is it better to call a func that sets state?
        return;
    }

    switch (m_cardsSelected)
    {
        case CardSelected::NoCard:
        case CardSelected::OneCard:
            break;
        case CardSelected::TwoCards:
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
                    m_attempts--;
                }

                m_firstCardIdx = -1;
                m_secondCardIdx = -1;
                m_cardsSelected = CardSelected::NoCard;

                if (m_numOfCardsMatched == m_grid->GetSize())
                    m_gameState = GameState::Ended;
            }
        default: ;
    }
}
// TODO: move to EndScene::Render()
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
    SDL_Texture* play = m_assetManager->GetTexture("UI_PlayButton");
    SDL_RenderTexture(m_renderer, play, nullptr, &m_uiPlayButtonRect);

    // QuitButton
    SDL_Texture* quit = m_assetManager->GetTexture("UI_QuitButton");
    SDL_RenderTexture(m_renderer, quit, nullptr, &m_uiQuitButtonRect);

    SDL_RenderPresent(m_renderer);
}
// NOTE: moved to StartScene::Render()
// void Game::UpdateStartScreen() const
// {
//     // Draw Start screen
//     SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
//     SDL_RenderClear(m_renderer);
//     SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
//
//     // Starting title
//     SDL_SetRenderScale(m_renderer, 4.f, 4.f);
//     const auto text = "MEMORIA HORRIFICA";
//     const float middle = ((static_cast<float>(m_windowWidth) / 2.0f) / 5.f) - (static_cast<float>(SDL_strlen(text)) * 5.f) / 1.55f;
//     SDL_RenderDebugText(m_renderer, middle, 25.0f, text);
//     SDL_SetRenderScale(m_renderer, 1.0f, 1.0f);
//
//     // PlayButton
//     SDL_Texture* play = m_assetManager->GetTexture("UI_PlayButton");
//     SDL_RenderTexture(m_renderer, play, nullptr, &m_uiPlayButtonRect);
//
//     // QuitButton
//     SDL_Texture* quit = m_assetManager->GetTexture("UI_QuitButton");
//     SDL_RenderTexture(m_renderer, quit, nullptr, &m_uiQuitButtonRect);
//
//     SDL_RenderPresent(m_renderer);
// }

// NOTE: moved to StartingScene::HandleEvent()
Game::GameCommand Game::HandleStartingState(const SDL_FPoint &p)
{
    if (SDL_PointInRectFloat(&p, &m_uiPlayButtonRect))
    {
        Run();
        return GameCommand::None;
    }
    if (SDL_PointInRectFloat(&p, &m_uiQuitButtonRect))
        return GameCommand::Quit;

    return GameCommand::None;
}
// TODO: move to EndScene::HandleEvent()
Game::GameCommand Game::HandleEndingState(const SDL_FPoint &p) const
{
    if (SDL_PointInRectFloat(&p, &m_uiPlayButtonRect))
        return GameCommand::Restart;

    if (SDL_PointInRectFloat(&p, &m_uiQuitButtonRect))
        return GameCommand::Quit;

    return GameCommand::None;
}
// TODO: this renders the scene and UI of GameScene. Move to GameScene
void Game::Render() const
{
    // Iterate over GridLayout and render
    SDL_Texture* cardBack = m_assetManager->GetTexture("Card_Back");
    SDL_Texture* cardRender{};
    const int size = static_cast<int>(m_grid->GetSize());
    for (int i = 0; i < size; ++i)
    {
        auto& card = m_grid->m_cards.at(i);
        auto& rect = m_grid->m_grid.at(i);

        if (card.state != CardState::FaceDown)
            cardRender = m_assetManager->GetTexture(card.frontKey);
        else
            cardRender = cardBack;

        SDL_RenderTexture(m_renderer, cardRender, nullptr, &rect);
    }
    RenderUI();
    SDL_RenderPresent(m_renderer);
}
// TODO: move to UI system or GameScene
void Game::RenderUI() const
{
    // Render Attempts
    const auto tex = m_assetManager->GetTexture("UI_Heart");
    for (int i = 0; i < m_attempts; ++i)
    {
        SDL_RenderTexture(m_renderer, tex, nullptr, &m_uiHeartRects[i]);
    }
    // TODO: Render Time
}
// NOTE: currently not used. Window is fixed size
void Game::Resize()
{
    SDL_GetWindowSizeInPixels(m_window, &m_windowWidth, &m_windowHeight);
    m_grid->InitGrid(m_windowWidth, m_windowHeight, TEX_WIDTH, TEX_HEIGHT);
}

void Game::HandleEvent(const SDL_Event &event) const
{
    m_sceneManager->HandleEvent(event);
}

// TODO: this function does more than one thing
Game::GameCommand Game::HitTest(const float x, const float y)
{
    // HitTests are locked if two Cards are currently faceUp
    if (m_cardsSelected == CardSelected::TwoCards)
        return GameCommand::None;

    const SDL_FPoint p = { x,y };
    switch (m_gameState)
    {
        case GameState::Starting:
            return HandleStartingState(p);
        case GameState::Running:
            break;
        case GameState::Ended:
            return HandleEndingState(p);
        default:
            return GameCommand::None;
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
            return GameCommand::None;

        m_soundSystem->PlaySfxSound(SoundSystem::SoundId::CardFlip);

        card.state = CardState::FaceUp;
        
        // Current Card is the first Card that was clicked
        if (m_cardsSelected == CardSelected::NoCard) 
        {
            m_firstCardIdx = i;
            m_cardsSelected = CardSelected::OneCard;
            return GameCommand::None;
        }

        m_secondCardIdx = i;
        m_cardsSelected = CardSelected::TwoCards;
        m_resolveCardsAtMs = SDL_GetTicks() + m_revealDelayMs;
    }
    return GameCommand::None;
}
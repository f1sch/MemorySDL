//#include "pch.h"

#include "AssetManager.h"
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

Game::Game(SDL_Window* window, SDL_Renderer* renderer, const int width, const int height)
    : m_window(window), m_renderer(renderer),
      m_windowWidth(width), m_windowHeight(height), m_gameContext(nullptr, nullptr, nullptr, nullptr, 0, 0, 0, 0)
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

    m_gameContext.renderer = m_renderer;
    m_gameContext.assetManager = m_assetManager.get();
    m_gameContext.grid = m_grid.get();
    m_gameContext.soundSystem = m_soundSystem.get();
    m_gameContext.windowWidth = m_windowWidth;
    m_gameContext.windowHeight = m_windowHeight;
    m_gameContext.texWidth = TEX_WIDTH;
    m_gameContext.texHeight = TEX_HEIGHT;

    m_sceneManager = std::make_unique<SceneManager>();
    m_sceneManager->RequestSceneChange(std::make_unique<StartScene>(
        *m_sceneManager, m_gameContext)
    );

    return 0;
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
    //return HitTest(x,y);
    return GameCommand::None;
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
    //SDL_RenderTexture(m_renderer, play, nullptr, &m_uiPlayButtonRect);

    // QuitButton
    SDL_Texture* quit = m_assetManager->GetTexture("UI_QuitButton");
    //SDL_RenderTexture(m_renderer, quit, nullptr, &m_uiQuitButtonRect);

    SDL_RenderPresent(m_renderer);
}

// TODO: move to EndScene::HandleEvent()
Game::GameCommand Game::HandleEndingState(const SDL_FPoint &p) const
{
    // if (SDL_PointInRectFloat(&p, &m_uiPlayButtonRect))
    //     return GameCommand::Restart;
    //
    // if (SDL_PointInRectFloat(&p, &m_uiQuitButtonRect))
    //     return GameCommand::Quit;

    return GameCommand::None;
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
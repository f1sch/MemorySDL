//#include "pch.h"

#include "AssetManager.h"
#include "Game.h"
#include "GridLayout.h"
#include "SceneManager.h"
#include "StartScene.h"
#include "SoundSystem.h"

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

int Game::Update() const
{
    const Uint64 now = SDL_GetTicks();
    // we'll have some textures move around over a few seconds.
    //const float direction = ((now % 2000) >= 1000) ? 1.0f : -1.0f;
    //const float scale = ((float)(((int)(now % 1000)) - 500) / 500.0f) * direction;

    if (m_sceneManager->ShouldQuit())
        return -1;

    m_sceneManager->Update(static_cast<float>(now));
    m_sceneManager->Render(m_renderer);

#ifdef __EMSCRIPTEN__
    if (m_soundSystem->IsMusicStarted())
        m_soundSystem->Update();
#else
    m_soundSystem->Update();
#endif
    return 0;
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
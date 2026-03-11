#include "scenes/StartScene.h"

#include "core/GameContext.h"
#include "scenes/GameScene.h"
#include "scenes/SceneManager.h"

#include <SDL3/SDL_render.h>

#include <memory>

void StartScene::InitUI()
{
    SDL_FRect tmpButton
        {
            m_context.windowWidth * 0.15f,
            m_context.windowHeight * 0.75f,
            m_context.texWidth * 0.5f,
            m_context.texHeight * 0.5f
        };
    SDL_Texture* play = m_context.assetManager->GetTexture("UI_PlayButton");
    m_ui.push_back(std::make_unique<Button>(
        tmpButton,
        play,
        [this]() {
            m_sceneManager.RequestSceneChange(
                std::make_unique<GameScene>(m_sceneManager, m_context));
        }
    ));

    SDL_FRect tmpButton2 =
    {
        m_context.windowWidth * 0.75f,
        m_context.windowHeight * 0.75f,
        m_context.texWidth * 0.5f,
        m_context.texHeight * 0.5f
    };
    SDL_Texture* play2 = m_context.assetManager->GetTexture("UI_QuitButton");
    m_ui.push_back(std::make_unique<Button>(
        tmpButton2,
        play2,
        [this]() {
            m_sceneManager.RequestQuit();
        }
    ));
}

void StartScene::HandleEvent(const SDL_Event &event)
{
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
#ifdef __EMSCRIPTEN__
        if (!m_context.soundSystem->IsMusicStarted()) {
            m_context.soundSystem->SetMusicStarted(true);
            m_context.soundSystem->Update();
        }
#endif
        for (const auto& element : m_ui)
        {
            element->HandleEvent(event);
        }
    }
}

void StartScene::Render(SDL_Renderer *renderer)
{
    // Draw Start screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    // Starting title
    SDL_SetRenderScale(renderer, 4.f, 4.f);
    const auto text = "MEMORIA HORRIFICA";
    const float middle = ((static_cast<float>(m_context.windowWidth) / 2.0f) / 5.f) - (static_cast<float>(SDL_strlen(text)) * 5.f) / 1.55f;
    SDL_RenderDebugText(renderer, middle, 25.0f, text);
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);

    for (const auto& element : m_ui)
    {
        element->Render(renderer);
    }

    SDL_RenderPresent(renderer);
}

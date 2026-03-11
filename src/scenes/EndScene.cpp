#include "scenes/EndScene.h"

#include "ui/Button.h"
#include "scenes/GameScene.h"

#include "memory"

void EndScene::InitUI()
{
    SDL_FRect tmpButton
        {
            static_cast<float>(m_context.windowWidth) * 0.15f,
            static_cast<float>(m_context.windowHeight) * 0.75f,
            static_cast<float>(m_context.texWidth) * 0.5f,
            static_cast<float>(m_context.texHeight) * 0.5f
        };
    SDL_Texture* play = m_context.assetManager->GetTexture("UI_PlayButton");
    m_ui.push_back(std::make_unique<Button>(
        tmpButton,
        play,
        [this]() {
            //m_context.grid->ShuffleDeck();
            //m_context.grid->ResetCardStates();
            m_sceneManager.RequestSceneChange(
                std::make_unique<GameScene>(m_sceneManager, m_context));
        }
    ));

    SDL_FRect tmpButton2 =
    {
        static_cast<float>(m_context.windowWidth) * 0.75f,
        static_cast<float>(m_context.windowHeight) * 0.75f,
        static_cast<float>(m_context.texWidth) * 0.5f,
        static_cast<float>(m_context.texHeight) * 0.5f
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

void EndScene::HandleEvent(const SDL_Event &event)
{
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        for (const auto& element : m_ui)
        {
            element->HandleEvent(event);
        }
    }
}

void EndScene::Update(float dt)
{

}

void EndScene::Render(SDL_Renderer *renderer)
{
    // Draw End screen
    SDL_SetRenderDrawColor(m_context.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_context.renderer);
    SDL_SetRenderDrawColor(m_context.renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    // Ending title
    SDL_SetRenderScale(m_context.renderer, 5.f, 5.f);
    const auto text = "GAME ENDED!";
    const float middle = ((static_cast<float>(m_context.windowWidth) / 2.0f) / 5.f) - (static_cast<float>(SDL_strlen(text)) * 5.f) / 1.25f;
    SDL_RenderDebugText(m_context.renderer, middle, middle, text);
    SDL_SetRenderScale(m_context.renderer, 1.0f, 1.0f);

    for (const auto& element : m_ui)
    {
        element->Render(renderer);
    }

    SDL_RenderPresent(m_context.renderer);
}

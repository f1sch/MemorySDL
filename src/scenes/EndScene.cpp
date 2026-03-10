#include "scenes/EndScene.h"

#include "scenes/GameScene.h"

void EndScene::HandleEvent(const SDL_Event &event)
{
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        SDL_FPoint p {event.button.x, event.button.y};

        if (SDL_PointInRectFloat(&p, &m_uiPlayButtonRect))
        {
            m_context.grid->ShuffleDeck();
            m_context.grid->ResetCardStates();
            m_sceneManager.RequestSceneChange(std::make_unique<GameScene>(m_sceneManager, m_context));
        }

        else if (SDL_PointInRectFloat(&p, &m_uiQuitButtonRect))
            m_sceneManager.RequestQuit();
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

    // PlayButton
    SDL_Texture* play = m_context.assetManager->GetTexture("UI_PlayButton");
    SDL_RenderTexture(m_context.renderer, play, nullptr, &m_uiPlayButtonRect);

    // QuitButton
    SDL_Texture* quit = m_context.assetManager->GetTexture("UI_QuitButton");
    SDL_RenderTexture(m_context.renderer, quit, nullptr, &m_uiQuitButtonRect);

    SDL_RenderPresent(m_context.renderer);
}

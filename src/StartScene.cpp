#include "StartScene.h"

#include "GameContext.h"
#include "GameScene.h"
#include "SceneManager.h"

#include <SDL3/SDL_render.h>

#include <memory>

void StartScene::HandleEvent(const SDL_Event &event)
{
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        const SDL_FPoint &p {event.button.x, event.button.y};

        if (SDL_PointInRectFloat(&p, &m_uiPlayButtonRect))
            m_sceneManager.ChangeScene(std::make_unique<GameScene>(m_sceneManager, m_context));

        else if (SDL_PointInRectFloat(&p, &m_uiQuitButtonRect))
        {
            // TODO: quit the game
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

    // PlayButton
    SDL_Texture* play = m_context.assetManager->GetTexture("UI_PlayButton");
    SDL_RenderTexture(renderer, play, nullptr, &m_uiPlayButtonRect);

    // QuitButton
    SDL_Texture* quit = m_context.assetManager->GetTexture("UI_QuitButton");
    SDL_RenderTexture(renderer, quit, nullptr, &m_uiQuitButtonRect);

    SDL_RenderPresent(renderer);
}

#include "ui/Button.h"

#include "SDL3/SDL_events.h"

void Button::HandleEvent(const SDL_Event& event)
{
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        const SDL_FPoint mouse_pos{ event.button.x, event.button.y };

        if (SDL_PointInRectFloat(&mouse_pos, &m_rect))
        {
            if (m_onClick)
                m_onClick();
        }
    }
}

void Button::Render(SDL_Renderer *renderer)
{
    SDL_RenderTexture(renderer, m_texture, nullptr, &m_rect);
}

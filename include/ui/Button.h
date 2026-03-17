#pragma once
#include "UIElement.h"

#include <functional>

class Button : public UIElement
{
public:
    Button(const SDL_FRect rect, SDL_Texture* texture,
        std::function<void()> onClick)
            : m_rect(rect), m_texture(texture), m_onClick(onClick)
    {}

    void HandleEvent(const SDL_Event& event) override;
    void Render(SDL_Renderer* renderer) override;

private:
    SDL_FRect m_rect;
    SDL_Texture* m_texture;
    std::function<void()> m_onClick;
};
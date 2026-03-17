#pragma once
#include "UIElement.h"

class UIImage : public UIElement
{
public:
    UIImage(SDL_Texture* texture, const SDL_FRect rect)
        : m_texture(texture), m_rect(rect)
    {}

    void SetScale(float scale);

    void Render(SDL_Renderer* renderer) override;

private:
    SDL_Texture* m_texture;
    SDL_FRect m_rect{};
};
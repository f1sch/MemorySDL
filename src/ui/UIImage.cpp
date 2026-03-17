#include "ui/UIImage.h"

void UIImage::SetScale(const float scale)
{
    m_rect.w *= scale;
    m_rect.h *= scale;
}

void UIImage::Render(SDL_Renderer *renderer)
{
    SDL_RenderTexture(renderer, m_texture, nullptr, &m_rect);
}

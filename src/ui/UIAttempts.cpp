#include "ui/UIAttempts.h"

void UIAttempts::Init()
{
    const auto size = m_maxAttempts;
    m_images.reserve(static_cast<size_t>(size));
    for (int i = 0; i < size; ++i)
    {
        const auto middleOfWindow = static_cast<float>(m_context.windowWidth) * 0.5f;
        const auto offset = static_cast<float>(m_context.texWidth * i);
        const auto middleOfTexture = static_cast<float>(m_context.texWidth * m_attempts) / 2;
        constexpr float padding = 0.001f;

        m_images.emplace_back(m_texture, SDL_FRect {
            (middleOfWindow + offset - middleOfTexture),
            static_cast<float>(m_context.windowHeight) * padding,
            static_cast<float>(m_context.texWidth),
            static_cast<float>(m_context.texHeight)
        });
    }
}

void UIAttempts::Render(SDL_Renderer *renderer)
{
    for (int i = 0; i < m_attempts; ++i)
    {
        m_images.at(i).Render(renderer);
    }
}

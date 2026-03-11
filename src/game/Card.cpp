#include "game/Card.h"

void Card::Render(SDL_Renderer *renderer) const
{
    SDL_Texture* tex =
        (m_state == CardState::FaceUp) ? m_front : m_back;

    SDL_RenderTexture(renderer, tex, nullptr, &m_rect);
}

bool Card::TryFlip()
{
    if (m_state != CardState::FaceDown)
        return false;

    m_state = CardState::FaceUp;
    return true;
}

void Card::SetMatched()
{
    m_state = CardState::Matched;
}

void Card::FlipDown()
{
    if (m_state == CardState::FaceUp)
        m_state = CardState::FaceDown;
}

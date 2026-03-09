#include "GameScene.h"

#include "Card.h"

#include "SDL3/SDL_timer.h"

void GameScene::HandleEvent(const SDL_Event &event)
{
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        // HitTests are locked if two Cards are currently faceUp
        if (m_cardsSelected == CardSelected::TwoCards)
            return;// GameCommand::None;

        const SDL_FPoint p = { event.button.x,event.button.y };

        const int size = static_cast<int>(m_context.grid->GetSize());
        for (int i = 0; i < size; ++i)
        {
            Card& card = m_context.grid->m_cards.at(i);

            // Only HitTest Cards that are face down
            if (card.state != CardState::FaceDown)
                continue;

            // Check if Grid was hit
            const auto& [gx, gy, gw, gh] = m_context.grid->m_grid.at(i);
            SDL_FRect rc{ gx, gy, gw, gh};
            if (!SDL_PointInRectFloat(&p, &rc))
                continue;

            // Do nothing if the same Card is clicked repeatedly
            if (m_cardsSelected == CardSelected::OneCard && m_firstCardIdx == i)
                return;// GameCommand::None;

            m_context.soundSystem->PlaySfxSound(SoundSystem::SoundId::CardFlip);

            card.state = CardState::FaceUp;

            // Current Card is the first Card that was clicked
            if (m_cardsSelected == CardSelected::NoCard)
            {
                m_firstCardIdx = i;
                m_cardsSelected = CardSelected::OneCard;
                return;// GameCommand::None;
            }

            m_secondCardIdx = i;
            m_cardsSelected = CardSelected::TwoCards;
            m_resolveCardsAtMs = SDL_GetTicks() + m_revealDelayMs;
        }
    }
}

void GameScene::Update(float dt)
{
    SDL_SetRenderDrawColor(m_context.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_context.renderer);

    if (m_attempts < 1)
    {
        m_gameState = GameState::Ended; // NOTE: is it better to call a func that sets state?
        return;
    }

    switch (m_cardsSelected)
    {
        case CardSelected::NoCard:
        case CardSelected::OneCard:
            break;
        case CardSelected::TwoCards:
            if (SDL_GetTicks() >= m_resolveCardsAtMs)
            {
                Card& a = m_context.grid->m_cards.at(m_firstCardIdx);
                Card& b = m_context.grid->m_cards.at(m_secondCardIdx);

                if (a.pairId == b.pairId)
                {
                    a.state = CardState::Matched;
                    b.state = CardState::Matched;
                    m_numOfCardsMatched += 2;
                }
                else
                {
                    a.state = CardState::FaceDown;
                    b.state = CardState::FaceDown;
                    m_attempts--;
                }

                m_firstCardIdx = -1;
                m_secondCardIdx = -1;
                m_cardsSelected = CardSelected::NoCard;

                if (m_numOfCardsMatched == m_context.grid->GetSize())
                    m_gameState = GameState::Ended;
            }
        default: ;
    }
}

void GameScene::Render(SDL_Renderer *renderer)
{
    // Iterate over GridLayout and render
    SDL_Texture* cardBack = m_context.assetManager->GetTexture("Card_Back");
    SDL_Texture* cardRender{};
    const int size = static_cast<int>(m_context.grid->GetSize());
    for (int i = 0; i < size; ++i)
    {
        auto& card = m_context.grid->m_cards.at(i);
        auto& rect = m_context.grid->m_grid.at(i);

        if (card.state != CardState::FaceDown)
            cardRender = m_context.assetManager->GetTexture(card.frontKey);
        else
            cardRender = cardBack;

        SDL_RenderTexture(m_context.renderer, cardRender, nullptr, &rect);
    }
    //RenderUI();
    // Render Attempts
    const auto tex = m_context.assetManager->GetTexture("UI_Heart");
    for (int i = 0; i < m_attempts; ++i)
    {
        SDL_RenderTexture(m_context.renderer, tex, nullptr, &m_uiHeartRects[i]);
    }
    // TODO: Render Time

    SDL_RenderPresent(m_context.renderer);
}

#include "scenes/GameScene.h"

#include "ui/Card.h"
#include "scenes/EndScene.h"

#include "SDL3/SDL_timer.h"

void GameScene::Init()
{
    // TODO: frontCards exists in Game.cpp and here.
    // This has to be refactored. This is only for testing now
    // As long as fronCards is the same in both files, it is ensured which cards are to be loaded
    const std::vector<std::string> frontCards
    { "Card_Skull", "Card_Coffin", "Card_Candle", "Card_Dagger", "Card_Bat", "Card_Door" };

    constexpr int numOfCards = 6; // set to number of cards that will be rendered
    std::vector<std::string> testingCards{};
    for (int i{}; i < numOfCards; ++i)
    {
        testingCards.push_back(frontCards[i]);
    }

    m_grid = std::make_unique<GridLayout>(numOfCards/2, 4); // TODO: rows, cols should be parameters for GridLayout::InitGrid()
    m_grid->BuildDeck(testingCards); // TODO: move to CardDeck class
    m_grid->ShuffleDeck(); // TODO: move to CardDeck class
    m_grid->InitGrid(m_context.windowWidth, m_context.windowHeight, m_context.texWidth, m_context.texHeight);

    constexpr auto size = static_cast<size_t>(MAX_ATTEMPTS);
    m_uiHeartRects.resize(size);
    for (size_t i = 0; i < size; ++i)
    {
        m_uiHeartRects[i].x = (static_cast<float>(m_context.windowWidth) * 0.5f + static_cast<float>(
                                   m_context.texWidth * i) - static_cast<float>(m_context.texWidth * m_attempts) /
                               2);
        m_uiHeartRects[i].y = static_cast<float>(m_context.windowHeight) * 0.001f;
        m_uiHeartRects[i].w = static_cast<float>(m_context.texWidth);
        m_uiHeartRects[i].h = static_cast<float>(m_context.texHeight);
    }
}

void GameScene::HandleEvent(const SDL_Event &event)
{
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        // HitTests are locked if two Cards are currently faceUp
        if (m_cardsSelected == CardSelected::TwoCards)
            return;

        const SDL_FPoint p = { event.button.x,event.button.y };

        const int size = static_cast<int>(m_grid->GetSize());
        for (int i = 0; i < size; ++i)
        {
            Card& card = m_grid->m_cards.at(i);

            // Only HitTest Cards that are face down
            if (card.state != CardState::FaceDown)
                continue;

            // Check if Grid was hit
            const auto& [gx, gy, gw, gh] = m_grid->m_grid.at(i);
            SDL_FRect rc{ gx, gy, gw, gh};
            if (!SDL_PointInRectFloat(&p, &rc))
                continue;

            // Do nothing if the same Card is clicked repeatedly
            if (m_cardsSelected == CardSelected::OneCard && m_firstCardIdx == i)
                return;

            m_context.soundSystem->PlaySfxSound(SoundSystem::SoundId::CardFlip);

            card.state = CardState::FaceUp;

            // Current Card is the first Card that was clicked
            if (m_cardsSelected == CardSelected::NoCard)
            {
                m_firstCardIdx = i;
                m_cardsSelected = CardSelected::OneCard;
                return;
            }

            m_secondCardIdx = i;
            m_cardsSelected = CardSelected::TwoCards;
            m_resolveCardsAtMs = SDL_GetTicks() + m_revealDelayMs;
        }
    }
}

void GameScene::Update(float dt)
{
    if (m_attempts < 1)
    {
        m_sceneManager.RequestSceneChange(std::make_unique<EndScene>(m_sceneManager, m_context));
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
                Card& a = m_grid->m_cards.at(m_firstCardIdx);
                Card& b = m_grid->m_cards.at(m_secondCardIdx);

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

                if (m_numOfCardsMatched == m_grid->GetSize())
                {
                    m_sceneManager.RequestSceneChange(std::make_unique<EndScene>(m_sceneManager, m_context));
                }
            }
        default: ;
    }
}

void GameScene::Render(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(m_context.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_context.renderer);

    // Iterate over GridLayout and render
    SDL_Texture* cardBack = m_context.assetManager->GetTexture("Card_Back");
    SDL_Texture* cardRender{};
    const int size = static_cast<int>(m_grid->GetSize());
    for (int i = 0; i < size; ++i)
    {
        auto& card = m_grid->m_cards.at(i);
        auto& rect = m_grid->m_grid.at(i);

        if (card.state != CardState::FaceDown)
            cardRender = m_context.assetManager->GetTexture(card.frontKey);
        else
            cardRender = cardBack;

        SDL_RenderTexture(m_context.renderer, cardRender, nullptr, &rect);
    }

    // Render Attempts
    const auto tex = m_context.assetManager->GetTexture("UI_Heart");
    for (int i = 0; i < m_attempts; ++i)
    {
        SDL_RenderTexture(m_context.renderer, tex, nullptr, &m_uiHeartRects[i]);
    }
    // TODO: Render Time

    SDL_RenderPresent(m_context.renderer);
}

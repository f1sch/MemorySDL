#include "scenes/GameScene.h"

#include "game/Card.h"
#include "scenes/EndScene.h"
#include "SDL3/SDL_log.h"

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
    m_grid->InitGrid(m_context.windowWidth, m_context.windowHeight, m_context.texWidth, m_context.texHeight);

    m_deck = std::make_unique<CardDeck>();
    m_deck->BuildDeck(testingCards);
    m_deck->ShuffleDeck();

    auto& cards = m_deck->GetCards();
    auto& rects = m_grid->GetRects();

    SDL_Texture* back = m_context.assetManager->GetTexture("Card_Back");
    m_cards.reserve(cards.size());
    for (size_t i = 0; i <cards.size(); ++i)
    {
        SDL_Texture* front = m_context.assetManager->GetTexture(cards.at(i).frontKey);
        m_cards.emplace_back(cards.at(i), rects.at(i), front, back);
    }

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

        const int size = static_cast<int>(m_cards.size());
        for (int i = 0; i < size; ++i)
        {
            Card& card = m_cards.at(i);

            // Only HitTest Cards that are face down
            if (card.IsFaceUp())
                continue;

            SDL_FRect rc = card.GetRect();
            if (!SDL_PointInRectFloat(&p, &rc))
                continue;
            //SDL_Log("Card clicked-> pos: %d, tex: %s", i, card.GetTexName()); // NOTE: testing
            // Do nothing if the same Card is clicked repeatedly
            if (m_cardsSelected == CardSelected::OneCard && m_firstCardIdx == i)
                return;

            m_context.soundSystem->PlaySfxSound(SoundSystem::SoundId::CardFlip);

            if (!card.TryFlip())
                continue;

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
                auto& a = m_cards[m_firstCardIdx];
                auto& b = m_cards[m_secondCardIdx];

                if (a.GetPairId() == b.GetPairId())
                {
                    a.SetMatched();
                    b.SetMatched();
                    m_numOfCardsMatched += 2;
                }
                else
                {
                    a.FlipDown();
                    b.FlipDown();
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

    for (auto& card : m_cards)
    {
        card.Render(m_context.renderer);
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

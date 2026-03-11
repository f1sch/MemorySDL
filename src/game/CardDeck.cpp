#include "game/CardDeck.h"

#include <algorithm>

// Creates a List of shuffled cards
void CardDeck::BuildDeck(const std::vector<std::string> &frontKeys)
{
    m_cards.clear();
    m_cards.reserve(frontKeys.size() * 2);
    //m_cards.resize(frontKeys.size() * 2);

    int pairId = 0;
    int uniqueId = 0;
    for (const auto& key : frontKeys)
    {
        CardData data {uniqueId++, pairId, key, 0.0f};
        m_cards.push_back(data);
        m_cards.push_back(data);
        // Card a; a.uniqueId = uniqueId++; a.pairId = pairId; a.frontKey = key;
        // Card b; b.uniqueId = uniqueId++; b.pairId = pairId; b.frontKey = key;
        // m_cards.push_back(b);
        ++pairId;
    }
}

void CardDeck::ShuffleDeck()
{
    std::ranges::shuffle(m_cards.begin(), m_cards.end(), m_rng);
}

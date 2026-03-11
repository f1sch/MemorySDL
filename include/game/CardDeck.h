#ifndef MEMORYSDL_CARDDECK_H
#define MEMORYSDL_CARDDECK_H

#include "game/Card.h"

#include <random>
#include <string>
#include <vector>

class CardDeck
{
public:
    CardDeck()
        :m_rng(std::random_device{}())
    {}

    void BuildDeck(const std::vector<std::string>& frontKeys);
    void ShuffleDeck();

    [[nodiscard]] const std::vector<CardData>& GetCards() const { return m_cards; }

private:
    std::vector<CardData> m_cards;
    std::mt19937 m_rng;
};


#endif //MEMORYSDL_CARDDECK_H
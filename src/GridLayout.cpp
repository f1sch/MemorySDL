//#include "pch.h"

#include "Card.h"
#include "GridLayout.h"

#include <SDL3/SDL_rect.h>

#include <algorithm>
#include <random>
#include <string>
#include <vector>

GridLayout::GridLayout(const int rows, const int columns)
    : m_rng(std::random_device{}()), m_rows(rows), m_columns(columns)
{
}

GridLayout::~GridLayout()
= default;

void GridLayout::InitGrid(const int width, const int height, const int texWidth, const int texHeight)
{
    constexpr float margin = 5.f;
    const float gridWidth = static_cast<float>(m_columns) * static_cast<float>(texWidth) + (static_cast<float>(m_columns) - 1) * margin;
    const float gridHeight = static_cast<float>(m_rows) * static_cast<float>(texHeight) + (static_cast<float>(m_rows) - 1) * margin;

    const float startX = (static_cast<float>(width) - gridWidth) * 0.5f;
    const float startY = (static_cast<float>(height) - gridHeight) * 0.5f;

    m_grid.clear();
    const int size = m_rows * m_columns;
    for (int i = 0; i < size; ++i)
    {
        const int row = i / m_columns;
        const int col = i % m_columns;

        m_grid.insert(
            { m_cards.at(i).uniqueId, SDL_FRect{
            startX + static_cast<float>(col) * (static_cast<float>(texWidth) + margin),
            startY + static_cast<float>(row) * (static_cast<float>(texHeight) + margin),
            static_cast<float>(texWidth), static_cast<float>(texHeight)
            } }
        );
    }
}

// Creates a List of shuffled cards
void GridLayout::BuildDeck(const std::vector<std::string>& frontKeys)
{
    m_cards.clear();
    m_cards.reserve(frontKeys.size() * 2);

    int pairId = 0;
    int uniqueId = 0;
    for (const auto& key : frontKeys)
    {
        Card a; a.uniqueId = uniqueId++; a.pairId = pairId; a.frontKey = key;
        Card b; b.uniqueId = uniqueId++; b.pairId = pairId; b.frontKey = key;
        m_cards.push_back(a);
        m_cards.push_back(b);
        ++pairId;
    }
}

void GridLayout::ShuffleDeck()
{
    std::ranges::shuffle(m_cards.begin(), m_cards.end(), m_rng);
}

void GridLayout::ResetCardStates()
{
    for (auto& card : m_cards)
    {
        card.state = CardState::FaceDown;
    }
}

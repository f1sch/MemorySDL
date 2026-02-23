#include "Card.h"
#include "GridLayout.h"

#include <SDL3/SDL_rect.h>

#include <algorithm>
#include <random>
#include <string>
#include <vector>

GridLayout::GridLayout(const int rows, const int columns)
    : m_rows(rows), m_columns(columns)
{
}

GridLayout::~GridLayout()
{
}

void GridLayout::InitGrid(const int width, const int height, const int texWidth, const int texHeight)
{
    float margin = 5.f;
    float gridWidth = m_columns * texWidth + (m_columns - 1) * margin;
    float gridHeight = m_rows * texHeight + (m_rows - 1) * margin;

    float startX = (width - gridWidth) * 0.5f;
    float startY = (height - gridHeight) * 0.5f;

    m_grid.clear();
    const int size = static_cast<size_t>(m_rows * m_columns);
    for (int i = 0; i < size; ++i)
    {
        int row = static_cast<int>(i) / m_columns;
        int col = static_cast<int>(i) % m_columns;

        m_grid.insert(
            { m_cards.at(i).uniqueId, SDL_FRect{
            startX + col * (texWidth + margin),
            startY + row * (texHeight + margin),
            (float)texWidth, (float)texHeight
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

    std::mt19937 rng{ std::random_device{}() };
    std::shuffle(m_cards.begin(), m_cards.end(), rng);
}
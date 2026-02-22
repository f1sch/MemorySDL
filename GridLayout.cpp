#include "GridLayout.h"
#include "Card.h"

#include <random>

GridLayout::GridLayout(const int rows, const int columns)
    : m_rows(rows), m_columns(columns)
{
    //m_grid.resize(static_cast<size_t>(rows * columns));
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

    //for (size_t i{}; i < m_grid.size(); ++i)
    m_grid.clear();
    for (size_t i{}; i < static_cast<size_t>(m_rows * m_columns); ++i)
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

        //m_grid.at(i) = SDL_FRect{
        //    startX + col * (texWidth + margin),
        //    startY + row * (texHeight + margin),
        //    (float)texWidth, (float)texHeight
        //};
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
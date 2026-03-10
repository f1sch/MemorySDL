#pragma once

#include "ui/Card.h"

#include <SDL3/SDL_rect.h>

#include <map>
#include <random>
#include <string>
#include <vector>


class GridLayout
{
public:
	GridLayout(int rows, int columns);
	~GridLayout();

	void InitGrid(int width, int height, int texWidth, int texHeight);
	[[nodiscard]] size_t GetSize() const { return m_grid.size(); }
	void BuildDeck(const std::vector<std::string>& frontKeys);
	void ShuffleDeck();
	void ResetCardStates();

	std::map<int, SDL_FRect> m_grid;
	std::vector<Card> m_cards;
private:
	std::mt19937 m_rng;
	int m_rows = -1;
	int m_columns = -1;
};
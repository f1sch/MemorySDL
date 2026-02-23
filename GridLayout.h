#pragma once
#include "Card.h"

#include <SDL3/SDL_rect.h>

#include <map>
#include <string>
#include <vector>

class GridLayout
{
	struct Element {
		int cardId = -1;
		SDL_FRect rect;
	}; // TODO: delete me?
public:
	GridLayout(const int rows, const int columns);
	~GridLayout();

	void InitGrid(const int width, const int height, const int texWidth, const int texHeight);
	size_t GetSize() const { return m_grid.size(); }
	void BuildDeck(const std::vector<std::string>& frontKeys);
	
	std::map<int, SDL_FRect> m_grid;
	std::vector<Card> m_cards;
private:
	int m_rows = -1;
	int m_columns = -1;
};
#pragma once

#include <SDL3/SDL_rect.h>

#include <string>
#include <vector>

class GridLayout
{
public:
	GridLayout(int rows, int columns);
	~GridLayout() = default;

	void InitGrid(int width, int height, int texWidth, int texHeight);

	[[nodiscard]] size_t GetSize() const { return m_grid.size(); }
	[[nodiscard]] const std::vector<SDL_FRect>& GetRects() const { return m_grid; }

private:
	std::vector<SDL_FRect> m_grid;

	int m_rows = -1;
	int m_columns = -1;
};

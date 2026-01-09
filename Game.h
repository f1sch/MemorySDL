#pragma once

#include "AssetManager.h"
#include "Card.h"

#include <SDL3/SDL.h>
#include <memory>

class Game
{
	enum class CardSelected { NoCard, OneCard, TwoCards };
public:
	Game(SDL_Window* window, SDL_Renderer* renderer, int width, int height);
	~Game();

	int Init();
	int Update();

	void Resize();

	void HitTest(float x, float y);
private:
	void ApplyGridLayout(const int rows, const int columns);
	void BuildDeck(const std::vector<std::string>& frontKeys);
private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	int m_windowWidth;
	int m_windowHeight;

	std::unique_ptr<AssetManager> m_assetManager;
	std::vector<Card> m_cards;

	// state
	CardSelected m_cardsSelected;
	int m_firstCardIdx = -1;
	int m_secondCardIdx = -1;
	Uint64 m_resolveCardsAtMs = 0;
	static constexpr Uint64 m_revealDelayMs = 800;
};


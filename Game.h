#pragma once

#include "AssetManager.h"
#include "Card.h"

#include <SDL3/SDL.h>
#include <memory>

class Game
{
public:
	Game(SDL_Window* window, SDL_Renderer* renderer, int width, int height);
	~Game();

	int Init();
	int Run();

	void HitTest(float x, float y);
private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	int m_windowWidth;
	int m_windowHeight;

	std::unique_ptr<AssetManager> m_assetManager;
	std::vector<Card> m_cards;
};


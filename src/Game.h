#pragma once

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>

#include <memory>

class AssetManager;
class GridLayout;
class SoundSystem;

class Game
{
	enum class CardSelected { NoCard, OneCard, TwoCards };
public:
	Game(SDL_Window* window, SDL_Renderer* renderer, int width, int height);
	~Game();

	int Init();
	void ShutdownGame();
	
	int Update();
	void UpdateGameplay();
	void UpdateEndScreen();

	void Resize();

	int HitTest(float x, float y);
private:
	// Window
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	int m_windowWidth;
	int m_windowHeight;

	// Systems
	std::unique_ptr<AssetManager> m_assetManager;
	std::unique_ptr<GridLayout> m_grid;
	std::unique_ptr<SoundSystem> m_soundSystem;

	// UI
	const char* m_playButtonText = "PLAY";
	const char* m_quitButtonText = "QUIT";
	SDL_FRect m_playButtonRect{};
	SDL_FRect m_quitButtonRect{};
	bool playPressed = false;
	bool quitPressed = false;

	// State
	enum class GameState { Running, Ended, Paused };
	GameState m_gameState;
	size_t m_numOfCardsMatched = 0;
	CardSelected m_cardsSelected;
	int m_firstCardIdx = -1;
	int m_secondCardIdx = -1;
	Uint64 m_resolveCardsAtMs = 0;
	static constexpr Uint64 m_revealDelayMs = 800;
};


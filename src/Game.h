#pragma once

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>

#include <memory>
#include <vector>

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
	void Start();
	void Run();
	void ShutdownGame() const;
	
	int Update();
	void UpdateGameplay();
	void UpdateEndScreen() const;
	void UpdateStartScreen() const;

	int HandleStartingState(const SDL_FPoint& p);
	[[nodiscard]] int HandleEndingState(const SDL_FPoint& p) const;

	void Render() const;
	void RenderUI() const;

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
	const char* m_uiPlayButtonText = "PLAY";
	const char* m_uiQuitButtonText = "QUIT";
	SDL_FRect m_uiPlayButtonRect{};
	SDL_FRect m_uiQuitButtonRect{};
	bool uiPlayPressed = false;
	bool uiQuitPressed = false;

	std::vector<SDL_FRect> m_uiHeartRects{};

	// State
	enum class GameState { Running, Ended, Paused, Starting };
	GameState m_gameState;
	size_t m_numOfCardsMatched = 0;
	CardSelected m_cardsSelected;
	int m_firstCardIdx = -1;
	int m_secondCardIdx = -1;
	Uint64 m_resolveCardsAtMs = 0;
	static constexpr Uint64 m_revealDelayMs = 800;

	int m_attempts = 5;
};


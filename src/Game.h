#pragma once
#include "GameContext.h"

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>

#include <memory>
#include <vector>

class AssetManager;
class GridLayout;
class SceneManager;
class SoundSystem;

class Game
{
public:
	enum class GameCommand { None, Quit, Restart };

	Game(SDL_Window* window, SDL_Renderer* renderer, int width, int height);
	~Game();

	int Init();
	void ShutdownGame() const;
	[[nodiscard]] int Update() const;
	void Resize();

	void HandleEvent(const SDL_Event& event) const;
	GameCommand OnMouseDown(float x, float y);

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
	std::unique_ptr<SceneManager> m_sceneManager;

	GameContext m_gameContext;
};


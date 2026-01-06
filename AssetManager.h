#pragma once
#include <SDL3/SDL.h>

#include <vector>

class AssetManager
{
public:
	AssetManager(SDL_Renderer* renderer);
	~AssetManager();

	int LoadTextures();

	// Getters
	std::vector<SDL_Texture*> GetTextures() { return m_textures; }
private:
	SDL_Renderer* m_renderer;
	std::vector<SDL_Texture*> m_textures;
	//int texture_width;
	//int texture_height;
};


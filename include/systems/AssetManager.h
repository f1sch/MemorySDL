#pragma once

#include <SDL3/SDL_render.h>

#include <unordered_map>
#include <string>

class AssetManager
{
public:
	explicit AssetManager(SDL_Renderer* renderer);
	~AssetManager();

	void LoadTexture(const std::string& key);

	// Getters
	SDL_Texture* GetTexture(const std::string& key) const;
private:
	SDL_Renderer* m_renderer;
	std::unordered_map<std::string, SDL_Texture*> m_textures;
};


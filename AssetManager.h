#pragma once
#include <SDL3/SDL.h>

#include <unordered_map>
#include <vector>
#include <string>

class AssetManager
{
public:
	AssetManager(SDL_Renderer* renderer);
	~AssetManager();

	int LoadTexture(const std::string& key, const std::string& path);

	// Getters
	SDL_Texture* GetTexture(const std::string& key) const;
private:
	SDL_Renderer* m_renderer;
	std::unordered_map<std::string, SDL_Texture*> m_textures;
};


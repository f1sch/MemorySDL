//#include "pch.h"
#include "AssetManager.h"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>

#include <string>
#include <unordered_map>

AssetManager::AssetManager(SDL_Renderer* renderer)
{
    m_renderer = renderer;
}

AssetManager::~AssetManager()
{
    for (const auto& t: m_textures)
    {
        SDL_DestroyTexture(t.second);
    }
}

void AssetManager::LoadTexture(const std::string& key)
{
    SDL_Surface* surface = nullptr;
    char* png_path = nullptr;
    const std::string path = "assets/";
    const std::string imageType = ".png";
    // Textures are pixel data that we upload to the video hardware for fast drawing. Lots of 2D
    // engines refer to these as "sprites." We'll do a static texture (upload once, draw many
    // times) with data from a bitmap file.

    // SDL_Surface is pixel data the CPU can access. SDL_Texture is pixel data the GPU can access. 
    // Load a .png into a surface, move it to a texture from there.
    
    SDL_asprintf(&png_path, "%s%s%s%s", SDL_GetBasePath(), path.c_str(), key.c_str(), imageType.c_str());  // allocate a string of the full file path
    surface = SDL_LoadPNG(png_path);
    if (!surface) {
        SDL_Log("Couldn't load bitmap: %s", SDL_GetError());
    }

    SDL_free(png_path);  // done with this, the file is loaded.

    SDL_Texture* tex = SDL_CreateTextureFromSurface(m_renderer, surface);
    if (!tex) {
        SDL_Log("Couldn't create static texture: %s", SDL_GetError());
    }
    
    SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);
    m_textures.insert({ key, tex });
    
    SDL_DestroySurface(surface);  // done with this, the texture has a copy of the pixels now.
}

SDL_Texture* AssetManager::GetTexture(const std::string& key) const
{
    const auto it = m_textures.find(key);
    
    if (it == m_textures.end())
        SDL_Log("Texture not found");
    
    return it->second;
}

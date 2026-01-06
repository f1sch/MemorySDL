#include "AssetManager.h"

AssetManager::AssetManager(SDL_Renderer* renderer)
{
    m_renderer = renderer;
    //m_textures = NULL;
    //texture_width = 0;
    //texture_height = 0;

}

AssetManager::~AssetManager()
{
    for (SDL_Texture* t : m_textures)
    {
        SDL_DestroyTexture(t);
    }
}

int AssetManager::LoadTextures()
{
    SDL_Surface* surface = NULL;
    char* png_path = NULL;
    // Textures are pixel data that we upload to the video hardware for fast drawing. Lots of 2D
    // engines refer to these as "sprites." We'll do a static texture (upload once, draw many
    // times) with data from a bitmap file.

    // SDL_Surface is pixel data the CPU can access. SDL_Texture is pixel data the GPU can access. 
    // Load a .png into a surface, move it to a texture from there.
    
    //auto tex = IMG_LoadTexture(renderer, "assets\\Sprite-Skull.png");
    SDL_asprintf(&png_path, "%sassets/Back.png", SDL_GetBasePath());  // allocate a string of the full file path
    surface = SDL_LoadPNG(png_path);
    if (!surface) {
        SDL_Log("Couldn't load bitmap: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_free(png_path);  // done with this, the file is loaded.

    //texture_width = surface->w;
    //texture_height = surface->h;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(m_renderer, surface);
    if (!tex) {
        SDL_Log("Couldn't create static texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    else
    {
        SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);
        m_textures.push_back(tex);
    }

    SDL_DestroySurface(surface);  // done with this, the texture has a copy of the pixels now.

    return 0;
}
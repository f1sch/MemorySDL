#include "Game.h"

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_mouse.h>

#include <memory>


// Use this renderer to draw into this window every frame.
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

constexpr auto WINDOW_WIDTH = 640;
constexpr auto WINDOW_HEIGHT = 480;

std::unique_ptr<Game> game;

// This function runs once at startup.
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    
    SDL_SetAppMetadata("Spooky Memory", "0.1", "com.example.renderer-textures");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Memory", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    
    game = std::make_unique<Game>(window, renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    game->Init();
    
    return SDL_APP_CONTINUE;  // carry on with the program!
}

// This function runs when a new event (mouse input, keypresses, etc) occurs.
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  // end the program, reporting success to the OS.
    }
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        SDL_Log("Current Mouse position is: (%f, %f)", event->motion.x, event->motion.y);
        game->HitTest(event->motion.x, event->motion.y);
    }
    return SDL_APP_CONTINUE;  // carry on with the program!
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    game->Run();

    return SDL_APP_CONTINUE;  // carry on with the program!
}

// This function runs once at shutdown.
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    
    // SDL will clean up the window/renderer for us.
}
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */

#include "pch.h"
#include "Game.h"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

#include <memory>

// Use this renderer to draw into this window every frame.
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

constexpr auto WINDOW_WIDTH = 640;
constexpr auto WINDOW_HEIGHT = 480;

std::unique_ptr<Game> game = nullptr;

// This function runs once at startup.
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_SetAppMetadata("Spooky Memory", "0.1", "com.example.spookymemory");


    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Memory", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    //SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);

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
    if (event->type == SDL_EVENT_WINDOW_RESIZED) {
        // TODO: resize layout for cards
        game->Resize();
    }
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        SDL_Log("Current Mouse position is: (%f, %f)", event->button.x, event->button.y);
        game->HitTest(event->button.x, event->button.y);
    }
    return SDL_APP_CONTINUE;  // carry on with the program!
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    game->Update();
    return SDL_APP_CONTINUE;  // carry on with the program!
}

// This function runs once at shutdown.
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    // SDL will clean up the window/renderer for us.
    game->ShutdownGame();
}
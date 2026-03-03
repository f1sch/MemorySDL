#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS

//#include "pch.h"
#include "Game.h"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

#include <memory>

// Use this renderer to draw into this window every frame.
static SDL_Window* g_window = NULL;
static SDL_Renderer* g_renderer = NULL;

constexpr auto WINDOW_WIDTH = 640;
constexpr auto WINDOW_HEIGHT = 480;

bool g_shouldQuit = false;
std::unique_ptr<Game> g_game = nullptr;

// This function runs once at startup.
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_SetAppMetadata("Spooky Memory", "0.1", "com.example.spookymemory");


    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) 
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // NOTE: Disallow window resizing for now. Develop on a fixed size window and add resizing later. 
    if (!SDL_CreateWindowAndRenderer("Memory", WINDOW_WIDTH, WINDOW_HEIGHT, 0 /*SDL_WINDOW_RESIZABLE*/, &g_window, &g_renderer)) 
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    //SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
    
    g_game = std::make_unique<Game>(g_window, g_renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    g_game->Init();
    g_game->Start();

    return SDL_APP_CONTINUE;  // carry on with the program!
}

// This function runs when a new event (mouse input, keypresses, etc) occurs.
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT) 
    {
        g_shouldQuit = true;
        return SDL_APP_SUCCESS;  // end the program, reporting success to the OS.
    }
    if (event->type == SDL_EVENT_WINDOW_RESIZED) 
    {
        // TODO: resize layout for cards; give new window size to g_game
        //g_game->Resize(width, height);
    }
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) 
    {
        //SDL_Log("Current Mouse position is: (%f, %f)", event->button.x, event->button.y);
        switch (const auto cmd = g_game->OnMouseDown(event->button.x, event->button.y))
        {
            case Game::GameCommand::Restart:
                // g_game.reset();
                // g_game = std::make_unique<Game>(g_window, g_renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
                // g_game->Init();
                // g_game->Run();
                g_game->Restart();
                g_game->Run();
                break;
            case Game::GameCommand::Quit:
                g_shouldQuit = true;
                break;
            default:
                break;
        }
    }
    return SDL_APP_CONTINUE;  // carry on with the program!
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    if (g_shouldQuit == true)
        return SDL_APP_SUCCESS;

    g_game->Update();
    
    return SDL_APP_CONTINUE;  // carry on with the program!
}

// This function runs once at shutdown.
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    // SDL will clean up the window/renderer for us.
    if (g_game)
        g_game.reset();

    if (g_renderer)
    {
        SDL_DestroyRenderer(g_renderer);
        g_renderer = nullptr;
    }
    if (g_window)
    {
        SDL_DestroyWindow(g_window);
        g_window = nullptr;
    }

    SDL_Quit();
}
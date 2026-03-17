#pragma once
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_render.h"

class UIElement
{
public:
    virtual ~UIElement() = default;

    virtual void HandleEvent(const SDL_Event& event) {}
    virtual void Update() {}
    virtual void Render(SDL_Renderer* renderer) = 0;
};
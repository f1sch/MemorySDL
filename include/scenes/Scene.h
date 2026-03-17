#pragma once
#include "core/GameContext.h"

#include "SDL3/SDL_render.h"

class Scene
{
public:
    explicit Scene(GameContext& context)
        : m_context(context)
    {}
    virtual ~Scene() = default;

    virtual void OnEnter() {}
    virtual void OnExit() {}

    virtual void HandleEvent(const SDL_Event& event) = 0;
    virtual void Update(float dt) = 0;
    virtual void Render(SDL_Renderer *renderer) = 0;

protected:
    GameContext& m_context;
};
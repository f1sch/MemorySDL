#pragma once
#include "scenes/Scene.h"

#include "ui/Button.h"
#include "scenes/SceneManager.h"

#include <memory>
#include <vector>

class StartScene : public Scene
{
public:
    explicit StartScene(SceneManager &manager, GameContext& context)
        : Scene(context), m_sceneManager(manager)
    {
        InitUI();
    }

    void InitUI();

    void HandleEvent(const SDL_Event &event) override;
    void Update(float dt) override {};
    void Render(SDL_Renderer *renderer) override;

private:
    SceneManager &m_sceneManager;

    // UI
    std::vector<std::unique_ptr<UIElement>> m_ui;
};
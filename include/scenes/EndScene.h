#ifndef MEMORYSDL_ENDSCENE_H
#define MEMORYSDL_ENDSCENE_H
#include "scenes/Scene.h"

#include "ui/UIElement.h"
#include "scenes/SceneManager.h"

#include <vector>

class EndScene : public Scene
{
public:
    explicit EndScene(SceneManager &manager, GameContext &context)
        : Scene(context), m_sceneManager(manager)
    {
        InitUI();
    }

    void InitUI();
    void HandleEvent(const SDL_Event &event) override;
    void Update(float dt) override;
    void Render(SDL_Renderer *renderer) override;

private:
    SceneManager &m_sceneManager;

    // UI
    std::vector<std::unique_ptr<UIElement>> m_ui;
};

#endif //MEMORYSDL_ENDSCENE_H
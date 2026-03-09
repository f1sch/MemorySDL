#ifndef MEMORYSDL_GAMESCENE_H
#define MEMORYSDL_GAMESCENE_H
#include "Scene.h"
#include "SceneManager.h"

class GameScene : public Scene
{
public:
    explicit GameScene(SceneManager &manager, GameContext &context)
        : Scene(context), m_sceneManager(manager), m_context(context)
    {
    }

    void HandleEvent(const SDL_Event &event) override;
    void Update(float dt) override;
    void Render(SDL_Renderer *renderer) override;

private:
    SceneManager &m_sceneManager;
    GameContext& m_context;
};


#endif //MEMORYSDL_GAMESCENE_H
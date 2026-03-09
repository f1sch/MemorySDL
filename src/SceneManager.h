#ifndef MEMORYSDL_SCENEMANAGER_H
#define MEMORYSDL_SCENEMANAGER_H
#include <memory>

#include "Scene.h"


class SceneManager
{
public:
    SceneManager() = default;

    void ChangeScene(std::unique_ptr<Scene> newScene);
    void HandleEvent(const SDL_Event &event) const;
    void Update(float dt) const;
    void Render(SDL_Renderer *renderer) const;

private:
    std::unique_ptr<Scene> m_currentScene;
};


#endif //MEMORYSDL_SCENEMANAGER_H
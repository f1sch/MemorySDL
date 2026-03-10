#ifndef MEMORYSDL_SCENEMANAGER_H
#define MEMORYSDL_SCENEMANAGER_H
#include "scenes/Scene.h"

#include <memory>

class SceneManager
{
public:
    SceneManager() = default;

    void RequestSceneChange(std::unique_ptr<Scene> scene);
    void RequestQuit();
    [[nodiscard]] bool ShouldQuit() const { return m_quitRequested; }

    void HandleEvent(const SDL_Event &event) const;
    void Update(float dt);
    void Render(SDL_Renderer *renderer) const;

private:
    std::unique_ptr<Scene> m_currentScene;
    std::unique_ptr<Scene> m_nextScene;

    bool m_quitRequested = false;
};

#endif //MEMORYSDL_SCENEMANAGER_H
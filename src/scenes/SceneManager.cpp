#include "scenes/SceneManager.h"

void SceneManager::RequestSceneChange(std::unique_ptr<Scene> scene)
{
    m_nextScene = std::move(scene);
}

void SceneManager::RequestQuit()
{
    m_quitRequested = true;
}

void SceneManager::HandleEvent(const SDL_Event &event) const
{
    if (m_currentScene)
        m_currentScene->HandleEvent(event);
}

void SceneManager::Update(const float dt)
{
    if (m_nextScene)
    {
        if (m_currentScene)
            m_currentScene->OnExit();

        m_currentScene = std::move(m_nextScene);
        m_currentScene->OnEnter();
        // TODO: return here? does this immediately run the update() of the nextScene beforce currentScene update() returns?
    }
    if (m_currentScene)
        m_currentScene->Update(dt);
}

void SceneManager::Render(SDL_Renderer *renderer) const
{
    if (m_currentScene)
        m_currentScene->Render(renderer);
}


#include "SceneManager.h"

void SceneManager::ChangeScene(std::unique_ptr<Scene> newScene)
{
    if (m_currentScene)
    {
        m_currentScene->OnExit();
    }

    m_currentScene = std::move(newScene);

    if (m_currentScene)
    {
        m_currentScene->OnEnter();
    }
}

void SceneManager::HandleEvent(const SDL_Event &event) const
{
    if (m_currentScene)
    {
        m_currentScene->HandleEvent(event);
    }
}

void SceneManager::Update(const float dt) const
{
    if (m_currentScene)
    {
        m_currentScene->Update(dt);
    }
}

void SceneManager::Render(SDL_Renderer *renderer) const
{
    if (m_currentScene)
    {
        m_currentScene->Render(renderer);
    }
}


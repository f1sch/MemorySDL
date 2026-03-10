#ifndef MEMORYSDL_STARTSCENE_H
#define MEMORYSDL_STARTSCENE_H
#include "scenes/Scene.h"

class SceneManager;

class StartScene : public Scene
{
public:
    explicit StartScene(SceneManager &manager, GameContext& context)
        : Scene(context), m_sceneManager(manager)
    {
        m_uiPlayButtonRect.x = static_cast<float>(m_context.windowWidth) * 0.15f;
        m_uiPlayButtonRect.y = static_cast<float>(m_context.windowHeight) * 0.75f;
        m_uiPlayButtonRect.w = static_cast<float>(m_context.texWidth) * 0.5f;
        m_uiPlayButtonRect.h = static_cast<float>(m_context.texHeight) * 0.5f;

        m_uiQuitButtonRect.x = static_cast<float>(m_context.windowWidth) * 0.75f;
        m_uiQuitButtonRect.y = static_cast<float>(m_context.windowHeight) * 0.75f;
        m_uiQuitButtonRect.w = static_cast<float>(m_context.texWidth) * 0.5f;
        m_uiQuitButtonRect.h = static_cast<float>(m_context.texHeight) * 0.5f;
    }

    void HandleEvent(const SDL_Event &event) override;
    void Update(float dt) override {};
    void Render(SDL_Renderer *renderer) override;

private:
    SceneManager &m_sceneManager;

    // UI
    const char* m_uiPlayButtonText = "PLAY";
    const char* m_uiQuitButtonText = "QUIT";
    SDL_FRect m_uiPlayButtonRect{};
    SDL_FRect m_uiQuitButtonRect{};
    bool uiPlayPressed = false;
    bool uiQuitPressed = false;
};

#endif //MEMORYSDL_STARTSCENE_H
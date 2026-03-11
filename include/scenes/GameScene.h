#ifndef MEMORYSDL_GAMESCENE_H
#define MEMORYSDL_GAMESCENE_H
#include "scenes/Scene.h"
#include "scenes/SceneManager.h"

constexpr auto MAX_ATTEMPTS = 5;

class GameScene : public Scene
{
public:
    explicit GameScene(SceneManager &manager, GameContext &context)
        : Scene(context), m_sceneManager(manager), m_numOfCardsMatched(0),
          m_cardsSelected(CardSelected::NoCard),
          m_firstCardIdx(-1),
          m_secondCardIdx(-1), m_attempts(MAX_ATTEMPTS),
          m_resolveCardsAtMs(0)
    {
        Init();
    }

    void Init();
    void HandleEvent(const SDL_Event &event) override;
    void Update(float dt) override;
    void Render(SDL_Renderer *renderer) override;

private:
    SceneManager &m_sceneManager;

    // State
    enum class CardSelected { NoCard, OneCard, TwoCards };
    size_t m_numOfCardsMatched;
    CardSelected m_cardsSelected;
    int m_firstCardIdx;
    int m_secondCardIdx;
    int m_attempts;

    Uint64 m_resolveCardsAtMs;
    static constexpr Uint64 m_revealDelayMs = 800;

    // UI
    std::vector<SDL_FRect> m_uiHeartRects; // TODO: refactor to UIElement
};

#endif //MEMORYSDL_GAMESCENE_H
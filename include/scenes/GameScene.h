#ifndef MEMORYSDL_GAMESCENE_H
#define MEMORYSDL_GAMESCENE_H
#include "scenes/Scene.h"
#include "scenes/SceneManager.h"

constexpr auto MAX_ATTEMPTS = 5;

class GameScene : public Scene
{
public:
    explicit GameScene(SceneManager &manager, GameContext &context)
        : Scene(context), m_sceneManager(manager), m_gameState(GameState::Paused), m_numOfCardsMatched(0),
          m_cardsSelected(CardSelected::NoCard),
          m_firstCardIdx(-1),
          m_secondCardIdx(-1), m_resolveCardsAtMs(0),
          m_attempts(MAX_ATTEMPTS)
    {
        constexpr auto size = static_cast<size_t>(MAX_ATTEMPTS);
        m_uiHeartRects.resize(size);
        for (size_t i = 0; i < size; ++i) {
            m_uiHeartRects[i].x = (static_cast<float>(m_context.windowWidth) * 0.5f + static_cast<float>(
                                       m_context.texWidth * i) - static_cast<float>(m_context.texWidth * m_attempts) /
                                   2);
            m_uiHeartRects[i].y = static_cast<float>(m_context.windowHeight) * 0.001f;
            m_uiHeartRects[i].w = static_cast<float>(m_context.texWidth);
            m_uiHeartRects[i].h = static_cast<float>(m_context.texHeight);
        }
    }

    void HandleEvent(const SDL_Event &event) override;
    void Update(float dt) override;
    void Render(SDL_Renderer *renderer) override;

private:
    SceneManager &m_sceneManager;

    enum class GameState { Running, Ended, Paused, Starting };
    GameState m_gameState;

    enum class CardSelected { NoCard, OneCard, TwoCards };
    size_t m_numOfCardsMatched;
    CardSelected m_cardsSelected;
    int m_firstCardIdx;
    int m_secondCardIdx;

    Uint64 m_resolveCardsAtMs;
    static constexpr Uint64 m_revealDelayMs = 800;

    std::vector<SDL_FRect> m_uiHeartRects;
    int m_attempts;
};

#endif //MEMORYSDL_GAMESCENE_H
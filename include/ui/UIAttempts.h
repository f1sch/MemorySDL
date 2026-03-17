#pragma once
#include "ui/UIImage.h"
#include "core/GameContext.h"

#include <vector>


class UIAttempts
{
public:
    UIAttempts(GameContext& context, SDL_Texture* texture, const int maxAttempts)
        : m_context(context), m_texture(texture), m_maxAttempts(maxAttempts), m_attempts(maxAttempts)
    {
        Init();
    }

    void Init();
    void SetAttempts(const int attempts) { m_attempts = attempts; }

    void Render(SDL_Renderer* renderer);

private:
    GameContext& m_context;
    SDL_Texture* m_texture;
    std::vector<UIImage> m_images {};
    int m_maxAttempts;
    int m_attempts;
};
#pragma once
#include "game/CardData.h"

#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"

class Card
{
public:
	enum class CardState { FaceDown, FaceUp, Matched };

	Card(const CardData& data, const SDL_FRect& rect, SDL_Texture* front, SDL_Texture* back)
		: m_data(data), m_rect(rect), m_front(front), m_back(back)
	{}

	void Render(SDL_Renderer* renderer) const;
	void HandleEvent(const SDL_Event& event);

	bool TryFlip();
	void SetMatched();
	void FlipDown();

	[[nodiscard]] bool IsFaceDown() const { return m_state == CardState::FaceDown; }
	[[nodiscard]] bool IsFaceUp() const { return m_state == CardState::FaceUp; }

	//[[nodiscard]] const char *GetTexName() const { return m_data.frontKey.c_str(); } // NOTE: for testing
	[[nodiscard]] int GetPairId() const { return m_data.pairId; }
	[[nodiscard]] SDL_FRect GetRect() const { return m_rect; }

private:
	CardData m_data;
	SDL_FRect m_rect;

	SDL_Texture* m_front;
	SDL_Texture* m_back;

	CardState m_state = CardState::FaceDown;
};
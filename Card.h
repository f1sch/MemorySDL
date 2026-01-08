#pragma once

#include <SDL3/SDL.h>
#include <string>

enum class CardState { FaceDown, FaceUp, Matched };

struct Card
{
	int uniqueId = -1;
	int pairId = -1;
	CardState state = CardState::FaceDown;
	std::string frontKey;
	SDL_FRect dst{}; // coords
	float rotation = 0.f;

	bool contains(float x, float y) const
	{
		return (x >= dst.x && x <= dst.x + dst.w &&
				y >= dst.y && y <= dst.y + dst.h);
	}
};
#pragma once

#include <SDL3/SDL.h>
#include <string>

enum class CardState { FaceDown, FaceUp, Matched };

struct Card
{
	std::string frontKey;
	CardState state = CardState::FaceDown;
	//SDL_Texture* texture = nullptr;
	SDL_FRect dst{}; // coords
	int id = -1;
	//bool hovered = false;
	//bool selected = false;

	bool contains(float x, float y) const
	{
		return (x >= dst.x && x <= dst.x + dst.w &&
				y >= dst.y && y <= dst.y + dst.h);
	}

	//void render(SDL_Renderer* renderer) const
	//{
	//	SDL_RenderTexture(renderer, texture, nullptr, &dst);
	//}
};
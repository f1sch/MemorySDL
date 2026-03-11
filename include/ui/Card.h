#pragma once

#include <string>

enum class CardState { FaceDown, FaceUp, Matched };

struct Card
{
	int uniqueId = -1;
	int pairId = -1;
	CardState state = CardState::FaceDown;
	std::string frontKey;
	float rotation = 0.f;
};

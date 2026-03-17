#pragma once
#include <string>

struct CardData
{
    int uniqueId = -1;
    int pairId = -1;
    std::string frontKey;
    float rotation = 0.f;
};
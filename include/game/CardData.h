#ifndef MEMORYSDL_CARDDATA_H
#define MEMORYSDL_CARDDATA_H
#include <string>

struct CardData
{
    int uniqueId = -1;
    int pairId = -1;
    std::string frontKey;
    float rotation = 0.f;
};

#endif //MEMORYSDL_CARDDATA_H
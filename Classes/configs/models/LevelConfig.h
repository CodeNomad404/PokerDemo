#ifndef __LEVEL_CONFIG_H__
#define __LEVEL_CONFIG_H__

#include <vector>
#include "models/CardModel.h"

struct LevelConfig
{
    int levelId = 1;
    std::vector<CardModel> stackCards;
    std::vector<CardModel> playfieldCards;
};

#endif
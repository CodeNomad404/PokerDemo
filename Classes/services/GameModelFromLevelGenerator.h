#ifndef __GAME_MODEL_FROM_LEVEL_GENERATOR_H__
#define __GAME_MODEL_FROM_LEVEL_GENERATOR_H__

#include "configs/models/LevelConfig.h"
#include "models/GameModel.h"

class GameModelFromLevelGenerator
{
public:
    static GameModel generateModel(const LevelConfig& levelConfig); // 根据LevelConfig生成GameModel，levelConfig:关卡配置对象
};

#endif

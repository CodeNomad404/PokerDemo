#ifndef __LEVEL_CONFIG_LOADER_H__
#define __LEVEL_CONFIG_LOADER_H__

#include <vector>
#include "configs/models/LevelConfig.h"

class LevelConfigLoader
{
public:
    LevelConfig load(int levelId) const;
};

#endif

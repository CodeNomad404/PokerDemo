#ifndef __CARD_RES_CONFIG_H__
#define __CARD_RES_CONFIG_H__

#include <string>

struct CardResConfig
{
    // 单张底图
    std::string backBackground = "res/card_general.png";

    // 花色和点数字图路径前缀
    std::string suitPattern = "res/suits/";
    std::string numberPattern = "res/number/";
    std::string fileExt = ".png";

    std::string buildSuitPath(const std::string& suitToken) const
    {
        return suitPattern + suitToken + fileExt;
    }

    std::string buildBigNumberPath(const std::string& colorToken, const std::string& rankToken) const
    {
        return numberPattern + "big_" + colorToken + "_" + rankToken + fileExt;
    }

    std::string buildSmallNumberPath(const std::string& colorToken, const std::string& rankToken) const
    {
        return numberPattern + "small_" + colorToken + "_" + rankToken + fileExt;
    }
};

#endif

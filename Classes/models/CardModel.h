#ifndef __CARD_MODEL_H__
#define __CARD_MODEL_H__

#include <string>

enum class CardArea
{
    Unknown = 0,
    Stack,
    Playfield,
    Top
};

// 花色类型
enum CardSuitType
{
    CST_NONE = -1,
    CST_CLUBS,      // 梅花
    CST_DIAMONDS,   // 方块
    CST_HEARTS,     // 红桃
    CST_SPADES,     // 黑桃
    CST_NUM_CARD_SUIT_TYPES
};

// 正面类型
enum CardFaceType
{
    CFT_NONE = -1,
    CFT_ACE,
    CFT_TWO,
    CFT_THREE,
    CFT_FOUR,
    CFT_FIVE,
    CFT_SIX,
    CFT_SEVEN,
    CFT_EIGHT,
    CFT_NINE,
    CFT_TEN,
    CFT_JACK,
    CFT_QUEEN,
    CFT_KING,
    CFT_NUM_CARD_FACE_TYPES
};

class CardModel
{
public:
    CardArea area = CardArea::Unknown;
    int cardId = 0;
    CardSuitType suit = CardSuitType::CST_SPADES;
    CardFaceType rank = CardFaceType::CFT_ACE;
    std::string suitPath;
    std::string bigRankPath;
    std::string smallRankPath;
    bool hasPresetPosition = false;
    float presetPosX = 0.0f;
    float presetPosY = 0.0f;
    float sourcePosX = 0.0f; // 用于动画的来源位置
    float sourcePosY = 0.0f;
    
    std::string serialize() const; // 将CardModel序列化为字符串
    static CardModel deserialize(const std::string& data); // 从字符串反序列化为CardModel
};

#endif

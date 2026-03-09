#ifndef __CARD_MATCH_SERVICE_H__
#define __CARD_MATCH_SERVICE_H__

#include "models/CardModel.h"

class CardMatchService
{
public:
    static bool canMatch(const CardModel& tableCard, const CardModel& topCard); // 判断桌面牌和手牌顶牌是否可以匹配，tableCard:桌面牌的CardModel对象，topCard:手牌顶牌的CardModel对象
};

#endif

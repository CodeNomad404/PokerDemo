#ifndef __CARD_CONTROLLER_H__
#define __CARD_CONTROLLER_H__

#include <vector>
#include "models/CardModel.h"
#include "models/GameModel.h"

class CardController
{
public:
    static void moveToTopCard(CardModel& sourceCard, CardModel& topCard, std::vector<CardModel>& topCards); // 将sourceCard移动到topCard,并将原topCard覆盖
};

#endif

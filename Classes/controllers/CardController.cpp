#include "controllers/CardController.h"

void CardController::moveToTopCard(CardModel& sourceCard, CardModel& topCard, std::vector<CardModel>& topCards)
{
    topCard = sourceCard;
    topCards.push_back(topCard); 
    topCard.area = CardArea::Top;
}

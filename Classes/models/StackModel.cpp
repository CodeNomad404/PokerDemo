#include "StackModel.h"

std::vector<CardModel> &StackModel::getHandCards()
{
    return _handCards;
}

const std::vector<CardModel> &StackModel::getHandCards() const
{
    return _handCards;
}

void StackModel::setHandCards(const std::vector<CardModel> &cards)
{
    _handCards = cards;
}

bool StackModel::isEmpty() const
{
    return _handCards.empty();
}

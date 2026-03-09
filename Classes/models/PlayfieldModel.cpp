#include "PlayfieldModel.h"

std::vector<CardModel> &PlayfieldModel::getTableCards()
{
    return _tableCards;
}

const std::vector<CardModel> &PlayfieldModel::getTableCards() const
{
    return _tableCards;
}

void PlayfieldModel::setTableCards(const std::vector<CardModel> &cards)
{
    _tableCards = cards;
}

bool PlayfieldModel::isEmpty() const
{
    return _tableCards.empty();
}

#include "services/GameModelFromLevelGenerator.h"

#include "models/CardModel.h"

GameModel GameModelFromLevelGenerator::generateModel(const LevelConfig& levelConfig)
{
    GameModel gameModel;
    gameModel.setLevelId(levelConfig.levelId);
    std::vector<CardModel> handCards = levelConfig.stackCards;
    std::vector<CardModel> tableCards = levelConfig.playfieldCards;

    CardModel topCard;
    if (!handCards.empty())
    {
        // 将手牌堆的第一张牌作为顶牌，并从手牌堆中移除
        topCard = handCards.front();
        topCard.area = CardArea::Top;
        handCards.erase(handCards.begin());
    }
    else
    {
        // 如果没有手牌，则创建一个默认的顶牌（实际项目中可以根据需要调整）
        topCard.cardId = 1;
        topCard.suit = CardSuitType::CST_CLUBS;
        topCard.rank = CardFaceType::CFT_FOUR;
        topCard.area = CardArea::Top;
    }

    for (auto& card : handCards)
    {
        card.area = CardArea::Stack;
    }

    for (auto& card : tableCards)
    {
        card.area = CardArea::Playfield;
    }

    gameModel.setHandCards(handCards); 
    gameModel.setTableCards(tableCards);
    gameModel.setTopHandCard(topCard);
    gameModel.getTopCards().push(topCard);

    return gameModel;
}

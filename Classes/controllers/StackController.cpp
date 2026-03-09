#include "controllers/StackController.h"

#include "models/GameModel.h"
#include "views/GameView.h"

StackController::StackController(GameModel* gameModel)
    : _gameModel(gameModel)
{
}

void StackController::init(GameModel* gameModel)
{
    _gameModel = gameModel;
}

void StackController::initView(GameView* gameView)
{
    _gameView = gameView;
}

StackMoveDecision StackController::handleCardClick(int cardId) const
{
    StackMoveDecision decision;
    if (_gameModel == nullptr)
    {
        return decision;
    }

    const int handIndex = _findCardIndexById(cardId);
    if (handIndex < 0)
    {
        return decision;
    }

    if (handIndex != 0)
    {
        return decision;
    }

    decision.canMove = true;
    decision.sourceIndex = handIndex;
    decision.undoModel.sourceArea = CardArea::Stack;
    decision.undoModel.sourceIndex = handIndex;
    return decision;
}

void StackController::undoMoveCardBack(CardModel movedBackCard, const UndoModel& undoModel) const
{
    if (_gameModel == nullptr)
    {
        return;
    }

    movedBackCard.area = CardArea::Stack;
    auto& handCards = _gameModel->getHandCards();
    int insertIndex = undoModel.sourceIndex;
    if (insertIndex < 0)
    {
        insertIndex = 0;
    }
    if (insertIndex > static_cast<int>(handCards.size()))
    {
        insertIndex = static_cast<int>(handCards.size());
    }
    handCards.insert(handCards.begin() + insertIndex, movedBackCard);
}

int StackController::_findCardIndexById(int cardId) const
{
    if (_gameModel == nullptr)
    {
        return -1;
    }

    const auto& handCards = _gameModel->getHandCards();
    for (int i = 0; i < static_cast<int>(handCards.size()); ++i)
    {
        if (handCards[i].cardId == cardId)
        {
            return i;
        }
    }

    return -1;
}

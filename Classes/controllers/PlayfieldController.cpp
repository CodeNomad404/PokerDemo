#include "controllers/PlayfieldController.h"

#include "models/GameModel.h"
#include "services/CardMatchService.h"
#include "views/GameView.h"

PlayfieldController::PlayfieldController(GameModel* gameModel)
    : _gameModel(gameModel)
{
}

void PlayfieldController::init(GameModel* gameModel)
{
    _gameModel = gameModel;
}

void PlayfieldController::initView(GameView* gameView)
{
    _gameView = gameView;
}

PlayfieldMoveDecision PlayfieldController::handleCardClick(int cardId, const CardModel* topCard) const
{
    PlayfieldMoveDecision decision;
    if (_gameModel == nullptr || topCard == nullptr)
    {
        return decision;
    }

    const int tableIndex = _findCardIndexById(cardId);
    if (tableIndex < 0)
    {
        return decision;
    }

    const auto& tableCards = _gameModel->getTableCards();
    if (!CardMatchService::canMatch(tableCards[tableIndex], *topCard))
    {
        return decision;
    }

    decision.canMove = true;
    decision.sourceIndex = tableIndex;
    decision.undoModel.sourceArea = CardArea::Playfield;
    decision.undoModel.sourceIndex = tableIndex;
    return decision;
}

void PlayfieldController::undoMoveCardBack(CardModel movedBackCard, const UndoModel& undoModel) const
{
    if (_gameModel == nullptr)
    {
        return;
    }

    movedBackCard.area = CardArea::Playfield;
    auto& tableCards = _gameModel->getTableCards();
    int insertIndex = undoModel.sourceIndex;
    if (insertIndex < 0)
    {
        insertIndex = 0;
    }
    if (insertIndex > static_cast<int>(tableCards.size()))
    {
        insertIndex = static_cast<int>(tableCards.size());
    }
    tableCards.insert(tableCards.begin() + insertIndex, movedBackCard);
}

int PlayfieldController::_findCardIndexById(int cardId) const
{
    if (_gameModel == nullptr)
    {
        return -1;
    }

    const auto& tableCards = _gameModel->getTableCards();
    for (int i = 0; i < static_cast<int>(tableCards.size()); ++i)
    {
        if (tableCards[i].cardId == cardId)
        {
            return i;
        }
    }

    return -1;
}

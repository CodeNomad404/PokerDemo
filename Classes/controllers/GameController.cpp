#include "controllers/GameController.h"

#include "controllers/CardController.h"
#include "models/GameModel.h"
#include "services/GameModelFromLevelGenerator.h"
#include "views/GameView.h"

GameController::GameController(GameModel* gameModel, GameView* gameView)
: _gameModel(gameModel)
, _gameView(gameView)
, _stackController(gameModel)
, _playfieldController(gameModel)
{
}

void GameController::startGame(int levelId)
{
    if (_gameModel == nullptr || _gameView == nullptr)
    {
        return;
    }

    const LevelConfig levelConfig = _levelConfigLoader.load(levelId);
    *_gameModel = GameModelFromLevelGenerator::generateModel(levelConfig);

    _stackController.init(_gameModel);
    _playfieldController.init(_gameModel);
    _undoManager.init();

    _stackController.initView(_gameView);
    _playfieldController.initView(_gameView);

    _bindInput();
    _refreshView();
}

void GameController::_bindInput()
{
    if (_gameModel == nullptr || _gameView == nullptr)
    {
        return;
    }

    _gameView->bind(_gameModel, this);
    _gameView->setHandCardClickCallback([this](int cardId) { _handCardClicked(cardId); });
    _gameView->setTableCardClickCallback([this](int cardId) { _tableCardClicked(cardId); });
    _gameView->setUndoClickCallback([this]() { _undoClicked(); });
}

void GameController::_handCardClicked(int cardId)
{
    const StackMoveDecision decision = _stackController.handleCardClick(cardId);
    if (!decision.canMove)
    {
        return;
    }

    _gameView->playMoveToTopAnimation(cardId, [this, decision]() {
        _replaceTopWithCard(CardArea::Stack, decision.sourceIndex);
        _undoManager.pushRecord(decision.undoModel);
        _refreshView();
    });
}

void GameController::_tableCardClicked(int cardId)
{
    const PlayfieldMoveDecision decision = _playfieldController.handleCardClick(cardId, _gameModel->getTopHandCard());
    if (!decision.canMove)
    {
        return;
    }

    _gameView->playMoveToTopAnimation(cardId, [this, decision]() {
        _replaceTopWithCard(CardArea::Playfield, decision.sourceIndex);
        _undoManager.pushRecord(decision.undoModel);
        _refreshView();
    });
}

void GameController::_undoClicked()
{
    if (!_undoManager.hasUndoRecord())
    {
        return;
    }

    const UndoModel undoModel = _undoManager.popRecord();
    _gameView->playUndoAnimation(undoModel, [this, undoModel]() {
        _applyUndoRecord(undoModel);
        _refreshView();
    });
}

void GameController::_replaceTopWithCard(CardArea sourceArea, int sourceIndex)
{
    if (_gameModel == nullptr)
    {
        return;
    }

    if(sourceArea == CardArea::Stack)
    {
        auto& handCards = _gameModel->getHandCards();
        if (sourceIndex < 0 || sourceIndex >= static_cast<int>(handCards.size()))
        {
            return;
        }

        CardModel movedCard = handCards[sourceIndex];
        movedCard.area = CardArea::Top;
        _gameModel->setTopHandCard(movedCard);
        _gameModel->getTopCards().push(movedCard);
        handCards.erase(handCards.begin() + sourceIndex);
    }
    else if (sourceArea == CardArea::Playfield)
    {
        auto& tableCards = _gameModel->getTableCards();
        if (sourceIndex < 0 || sourceIndex >= static_cast<int>(tableCards.size()))
        {
            return;
        }

        CardModel movedCard = tableCards[sourceIndex];
        movedCard.area = CardArea::Top;
        _gameModel->setTopHandCard(movedCard);
        _gameModel->getTopCards().push(movedCard);
        tableCards.erase(tableCards.begin() + sourceIndex);
    }
}

void GameController::_applyUndoRecord(const UndoModel& undoModel)
{
    if (_gameModel == nullptr)
    {
        return;
    }

    CardModel* currentTopCard = _gameModel->getTopHandCard();
    if (currentTopCard == nullptr)
    {
        return;
    }

    CardModel movedBackCard = *currentTopCard;

    auto& topCards = _gameModel->getTopCards();
    if (!topCards.empty())
    {
        topCards.pop();
    }

    if (!topCards.empty())
    {
        _gameModel->setTopHandCard(topCards.top());
    }
    else
    {
        _gameModel->setTopHandCard(CardModel());
    }

    if (undoModel.sourceArea == CardArea::Stack)
    {
        _stackController.undoMoveCardBack(movedBackCard, undoModel);
    }
    else if (undoModel.sourceArea == CardArea::Playfield)
    {
        _playfieldController.undoMoveCardBack(movedBackCard, undoModel);
    }
}

void GameController::_refreshView() 
{
    _gameView->renderFromModel(*_gameModel);
}

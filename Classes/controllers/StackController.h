#ifndef __STACK_CONTROLLER_H__
#define __STACK_CONTROLLER_H__

#include "models/UndoModel.h"

class GameModel;
class CardModel;
class GameView;

struct StackMoveDecision
{
    bool canMove = false;
    int sourceIndex = -1;
    UndoModel undoModel;
};

class StackController
{
public:
    StackController(GameModel* gameModel); // 构造函数，接受GameModel的指针

    void init(GameModel* gameModel);
    void initView(GameView* gameView);

    StackMoveDecision handleCardClick(int cardId) const; // 处理手牌被点击的逻辑，cardId:被点击的牌的cardId
    void undoMoveCardBack(CardModel movedBackCard, const UndoModel& undoModel) const; // 根据撤销记录将牌移回手牌堆，movedBackCard:被移回的牌的CardModel对象，undoModel:撤销记录对象

private:
    int _findCardIndexById(int cardId) const;

    GameModel* _gameModel;
    GameView* _gameView = nullptr;
};

#endif

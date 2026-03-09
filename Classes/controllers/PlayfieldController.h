#ifndef __PLAYFIELD_CONTROLLER_H__
#define __PLAYFIELD_CONTROLLER_H__

#include "models/UndoModel.h"

class GameModel;
class CardModel;
class GameView;

struct PlayfieldMoveDecision
{
    bool canMove = false;
    int sourceIndex = -1;
    UndoModel undoModel;
};

class PlayfieldController
{
public:
    PlayfieldController(GameModel* gameModel); // 构造函数，接受GameModel的指针

    void init(GameModel* gameModel); // 初始化PlayfieldController，接受GameModel的指针
    void initView(GameView* gameView);

    PlayfieldMoveDecision handleCardClick(int cardId, const CardModel* topCard) const; // 处理桌面牌被点击的逻辑，cardId:被点击的牌的cardId，topCard:当前手牌顶牌的CardModel指针

    void undoMoveCardBack(CardModel movedBackCard, const UndoModel& undoModel) const; // 根据撤销记录将牌移回桌面，movedBackCard:被移回的牌的CardModel对象，undoModel:撤销记录对象

private:
    int _findCardIndexById(int cardId) const;

    GameModel* _gameModel;
    GameView* _gameView = nullptr;
};

#endif

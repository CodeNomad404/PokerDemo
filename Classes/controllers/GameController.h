#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include <stack>
#include "configs/loaders/LevelConfigLoader.h"
#include "controllers/PlayfieldController.h"
#include "controllers/StackController.h"
#include "managers/UndoManager.h"


class GameModel;
class GameView;

class GameController
{
public:
    GameController(GameModel* gameModel, GameView* gameView); // 构造函数，接受GameModel和GameView的指针

    void startGame(int levelId); // 游戏启动入口：按关卡ID加载模型并完成控制器/视图初始化

private:
    void _bindInput(); // 绑定GameView的输入回调函数，如手牌点击、桌面牌点击、回退按钮点击等
    
    void _handCardClicked(int cardId); // 处理手牌被点击的逻辑，cardId:被点击的牌的cardId
    void _tableCardClicked(int cardId); // 处理桌面牌被点击的逻辑，cardId:被点击的牌的cardId
    void _undoClicked(); // 处理回退按钮被点击的逻辑

    void _replaceTopWithCard(CardArea sourceArea, int sourceIndex); // 将指定区域的牌移动到手牌顶牌位置，sourceArea:牌来源区域（手牌或桌面牌），sourceIndex:牌在来源区域中的索引
    
    void _applyUndoRecord(const UndoModel& undoModel); // 根据撤销记录恢复GameModel状态，undoModel:撤销记录对象
    
    void _refreshView(); // 刷新GameView显示以反映当前GameModel状态

    GameModel* _gameModel;
    GameView* _gameView;
    StackController _stackController;
    PlayfieldController _playfieldController;
    UndoManager _undoManager;
    LevelConfigLoader _levelConfigLoader;
};

#endif

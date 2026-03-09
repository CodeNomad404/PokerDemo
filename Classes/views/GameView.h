#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

#include <functional>
#include <unordered_map>
#include <vector>

#include "cocos2d.h"
#include "models/CardModel.h"

class GameController;
class GameModel;
class UndoModel;
class CardView;

class GameView : public cocos2d::Layer
{
public:
    CREATE_FUNC(GameView);

    bool init() override;

    void bind(const GameModel* gameModel, const GameController* gameController); // 绑定GameModel和GameController，GameView通过它们获取数据和调用游戏逻辑方法
    void renderFromModel(const GameModel& gameModel); // 根据GameModel更新GameView的显示

    // 设置回调函数
    void setHandCardClickCallback(const std::function<void(int)>& callback);
    void setTableCardClickCallback(const std::function<void(int)>& callback);
    void setUndoClickCallback(const std::function<void()>& callback); // 设置回退按钮点击回调

    void playMoveToTopAnimation(int cardId, const std::function<void()>& onFinished); // 播放移动到顶部的动画
    void playUndoAnimation(const UndoModel& undoModel, const std::function<void()>& onFinished); // 播放回退动画

private:
    void _createStaticUi(); // 创建静态UI元素，如标题、提示文本等
    void _clearCardViews(); // 清除当前所有的CardView，准备重新创建
    void _rebuildCardViews(const GameModel& gameModel); // 根据GameModel重新创建CardView并添加到场景中
    void _rebuildHandCardViews(const std::vector<CardModel>& handCards); 
    void _rebuildTableCardViews(const std::vector<CardModel>& tableCards);
    void _playStackUndoAnimation(const UndoModel& undoModel, cocos2d::Vec2& targetPos);
    void _playTableUndoAnimation(const UndoModel& undoModel, cocos2d::Vec2& targetPos);

    const GameModel* _gameModel = nullptr; // 指向当前的GameModel
    const GameController* _gameController = nullptr; // 指向GameController

    cocos2d::Vec2 _topAnchor; // 顶部牌的锚点
    cocos2d::Vec2 _handAnchor; // 手牌的锚点
    cocos2d::Vec2 _tableAnchor; // 桌面牌的锚点
    
    const float kCardWidth = 90.0f;
    const float kCardHeight = 120.0f;

    CardView* _topCardView = nullptr; 
    std::vector<CardView*> _handCardViews; 
    std::vector<CardView*> _tableCardViews; 
    std::unordered_map<int, CardView*> _cardViewById; // 根据cardId快速查找对应的CardView

    //游戏布局
    const float kMainAreaRatio = 1500.0f / 2080.0f; // 上区
    const float kPileAreaRatio = 580.0f / 2080.0f;  // 下区

    // 回调函数
    std::function<void(int)> _handCardClickCallback;
    std::function<void(int)> _tableCardClickCallback;
    std::function<void()> _undoClickCallback;

    bool _isUndoAnimating = false;
    std::function<void()> _pendingUndoFinished;
};

#endif

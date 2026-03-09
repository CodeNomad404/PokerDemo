#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include <functional>

#include "cocos2d.h"

#include "models/CardModel.h"

class CardView : public cocos2d::Node
{
public:
    CREATE_FUNC(CardView);
    const float kMoveDuration = 0.5f;
    const float kCardWidth = 180.0f;
    const float kCardHeight = 240.0f;

    bool init() override; // 初始化CardView，创建背景和点数/花色贴图

    void setCardModel(const CardModel& cardModel); // 根据CardModel更新CardView的显示

    int getCardId() const; 

    void setClickCallback(const std::function<void(int)>& callback); // 设置点击回调函数
    void playMoveTo(const cocos2d::Vec2& targetPosition, const std::function<void()>& onFinished);// 播放移动动画，移动到targetPosition，动画结束后调用onFinished回调

private:
    void _registerTouch(); // 注册触摸事件，点击时调用_clickCallback

    CardModel _cardModel; // 当前CardView对应的CardModel

    // 显示用的子节点
    cocos2d::Sprite* _backgroundSprite = nullptr; 
    cocos2d::Sprite* _smallNumberSprite = nullptr; // 小号点数
    cocos2d::Sprite* _bigNumberSprite = nullptr; // 大号点数
    cocos2d::Sprite* _suitSprite = nullptr; 
    std::function<void(int)> _clickCallback;
};

#endif

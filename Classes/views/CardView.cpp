#include <string>
#include "configs/models/CardResConfig.h"
#include "models/CardModel.h"
#include "CardView.h"

USING_NS_CC;

bool CardView::init()
{
    if (!Node::init())
    {
        return false;
    }

    const CardResConfig cardResConfig;

    // 初始化扑克牌背景
    _backgroundSprite = Sprite::create(cardResConfig.backBackground);
    if (_backgroundSprite != nullptr)
    {
        const Size backgroundSize = _backgroundSprite->getContentSize();
        if (backgroundSize.width > 0.0f && backgroundSize.height > 0.0f)
        {
            _backgroundSprite->setScaleX(kCardWidth / backgroundSize.width);
            _backgroundSprite->setScaleY(kCardHeight / backgroundSize.height);
        }
        _backgroundSprite->setAnchorPoint(Vec2::ZERO);
        addChild(_backgroundSprite);
    }

    setContentSize(Size(kCardWidth, kCardHeight));
    setAnchorPoint(Vec2(0.5f, 0.5f));

    // 初始化点数和花色精灵
    _smallNumberSprite = Sprite::create(cardResConfig.buildSmallNumberPath("black", "A"));
    if (_smallNumberSprite != nullptr)
    {
        const Size smallNumberSize = _smallNumberSprite->getContentSize();
        if (smallNumberSize.height > 0.0f)
        {
            _smallNumberSprite->setScale((kCardWidth*0.25f) / smallNumberSize.height);
        }
        _smallNumberSprite->setPosition(Vec2(kCardWidth*0.15f, kCardHeight*0.85f));
        addChild(_smallNumberSprite, 1);
    }

    _bigNumberSprite = Sprite::create(cardResConfig.buildBigNumberPath("black", "A"));
    if (_bigNumberSprite != nullptr)
    {
        const Size bigNumberSize = _bigNumberSprite->getContentSize();
        if (bigNumberSize.height > 0.0f)
        {
            _bigNumberSprite->setScale((kCardWidth * 0.5f) / bigNumberSize.height);
        }
        _bigNumberSprite->setPosition(Vec2(kCardWidth * 0.5f, kCardHeight * 0.7f));
        addChild(_bigNumberSprite, 1);
    }

    _suitSprite = Sprite::create(cardResConfig.buildSuitPath("spade"));
    if (_suitSprite != nullptr)
    {
        const Size suitSize = _suitSprite->getContentSize();
        if (suitSize.height > 0.0f)
        {
            _suitSprite->setScale((kCardWidth * 0.5f) / suitSize.height);
        }
        _suitSprite->setPosition(Vec2(kCardWidth * 0.5f, kCardHeight * 0.3f));
        addChild(_suitSprite, 1);
    }

    _registerTouch();

    return true;
}

void CardView::setCardModel(const CardModel& cardModel)
{
    _cardModel = cardModel;

    if (_smallNumberSprite == nullptr || _bigNumberSprite == nullptr || _suitSprite == nullptr)
    {
        return;
    }

    _smallNumberSprite->setTexture(cardModel.smallRankPath);
    _bigNumberSprite->setTexture(cardModel.bigRankPath);
    _suitSprite->setTexture(cardModel.suitPath);
}

int CardView::getCardId() const
{
    return _cardModel.cardId;
}

void CardView::setClickCallback(const std::function<void(int)>& callback)
{
    _clickCallback = callback;
}

void CardView::playMoveTo(const cocos2d::Vec2& targetPosition, const std::function<void()>& onFinished)
{
    
    stopAllActions();
    auto moveAction = MoveTo::create(kMoveDuration, targetPosition);
    auto finishAction = CallFunc::create([onFinished]() {
        if (onFinished)
        {
            onFinished();
        }
    });
    // sequence动作会在moveAction完成后调用finishAction
    runAction(Sequence::create(moveAction, finishAction, nullptr));
}

void CardView::_registerTouch()
{
    auto* touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);

    touchListener->onTouchBegan = [this](Touch* touch, Event*) {
        const Vec2 localPoint = convertToNodeSpace(touch->getLocation());
        const Rect bounds(0.0f, 0.0f, getContentSize().width, getContentSize().height);
        return bounds.containsPoint(localPoint);
    };

    touchListener->onTouchEnded = [this](Touch* touch, Event*) {
        const Vec2 localPoint = convertToNodeSpace(touch->getLocation());
        const Rect bounds(0.0f, 0.0f, getContentSize().width, getContentSize().height);
        if (bounds.containsPoint(localPoint) && _clickCallback)
        {
            _clickCallback(_cardModel.cardId);
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}
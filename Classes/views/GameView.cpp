#include <algorithm>
#include "GameView.h"
#include "models/GameModel.h"
#include "models/UndoModel.h"
#include "views/CardView.h"
USING_NS_CC;

bool GameView::init()
{
    if (!Layer::init())
    {
        return false;
    }

    _createStaticUi();
    return true;
}

void GameView::bind(const GameModel* gameModel, const GameController* gameController)
{
    _gameModel = gameModel;
    _gameController = gameController;
}

void GameView::renderFromModel(const GameModel& gameModel)
{
    _clearCardViews();
    _rebuildCardViews(gameModel);
}

void GameView::setHandCardClickCallback(const std::function<void(int)>& callback)
{
    _handCardClickCallback = callback;
}

void GameView::setTableCardClickCallback(const std::function<void(int)>& callback)
{
    _tableCardClickCallback = callback;
}

void GameView::setUndoClickCallback(const std::function<void()>& callback)
{
    _undoClickCallback = callback;
}

void GameView::playMoveToTopAnimation(int cardId, const std::function<void()>& onFinished)
{
    auto iter = _cardViewById.find(cardId);
    if (iter == _cardViewById.end())
    {
        if (onFinished)
        {
            onFinished();
        }
        return;
    }

    CardView* movingCardView = iter->second;
    const bool wasHandCard = std::find(_handCardViews.begin(), _handCardViews.end(), movingCardView) != _handCardViews.end();

    _cardViewById.erase(iter);
    _handCardViews.erase(std::remove(_handCardViews.begin(), _handCardViews.end(), movingCardView), _handCardViews.end());
    _tableCardViews.erase(std::remove(_tableCardViews.begin(), _tableCardViews.end(), movingCardView), _tableCardViews.end());
    if (_topCardView == movingCardView)
    {
        _topCardView = nullptr;
    }

    movingCardView->setClickCallback(nullptr);
    movingCardView->setLocalZOrder(9999);

    if (wasHandCard)
    {
        const float defaultGap = 120.0f;
        const float gap = _handCardViews.empty() ? defaultGap : 540.0f / static_cast<float>(_handCardViews.size());
        const float handStartX = _handAnchor.x + gap * (static_cast<float>(_handCardViews.size()) - 1.0f) * 0.5f;
        for (int i = 0; i < static_cast<int>(_handCardViews.size()); ++i)
        {
            _handCardViews[i]->setPosition(Vec2(handStartX - static_cast<float>(i) * gap, _handAnchor.y));
        }
    }

    movingCardView->playMoveTo(_topAnchor, [movingCardView, onFinished]() {
        movingCardView->removeFromParent();
        if (onFinished)
        {
            onFinished();
        }
    });
}

void GameView::playUndoAnimation(const UndoModel& undoModel, const std::function<void()>& onFinished)
{
    if (_isUndoAnimating && _topCardView != nullptr)
    {
        _topCardView->stopAllActions();
        _isUndoAnimating = false;
        if (_pendingUndoFinished)
        {
            auto previousFinished = _pendingUndoFinished;
            _pendingUndoFinished = nullptr;
            previousFinished();
        }
    }

    if (_topCardView == nullptr || _gameModel == nullptr)
    {
        if (onFinished)
        {
            onFinished();
        }
        return;
    }

    Vec2 targetPos = _topAnchor;

    if (undoModel.sourceArea == CardArea::Stack)
    {
        _playStackUndoAnimation(undoModel, targetPos);
    }
    else if (undoModel.sourceArea == CardArea::Playfield)
    {
        _playTableUndoAnimation(undoModel, targetPos);
    }

    _isUndoAnimating = true;
    _pendingUndoFinished = onFinished;
    _topCardView->playMoveTo(targetPos, [this]() {
        _isUndoAnimating = false;
        if (_pendingUndoFinished)
        {
            auto finished = _pendingUndoFinished;
            _pendingUndoFinished = nullptr;
            finished();
        }
    });
}

void GameView::_createStaticUi()
{
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto origin = Director::getInstance()->getVisibleOrigin();

    auto playfieldBackground = LayerColor::create(Color4B(255, 165, 0, 255), visibleSize.width, visibleSize.height);
    addChild(playfieldBackground);
    auto stackBackground = LayerColor::create(Color4B(128, 0, 128, 255), visibleSize.width, visibleSize.height * kPileAreaRatio);
    addChild(stackBackground);

    const float pileAreaHeight = visibleSize.height * kPileAreaRatio;
    const float mainAreaHeight = visibleSize.height * kMainAreaRatio;
    const float pileTopY = origin.y + pileAreaHeight;

    // 主牌区(上)中心
    const float mainCenterY = pileTopY + mainAreaHeight * 0.5f;
    // 堆牌区(下)中心
    const float pileCenterY = origin.y + pileAreaHeight * 0.5f;

    // 顶牌和手牌在堆牌区，手牌密集放在左侧,顶牌单独放在右侧,桌面牌在主牌区
    
    _topAnchor = Vec2(origin.x + visibleSize.width * 0.75f, pileCenterY);
    _handAnchor = Vec2(origin.x + visibleSize.width * 0.25f,pileCenterY);
    _tableAnchor = Vec2(origin.x + visibleSize.width * 0.5f, mainCenterY);

    auto undoLabel = Label::createWithSystemFont("Undo", "Arial", 30);
    // 回退按钮放在右下角
    auto undoItem = MenuItemLabel::create(undoLabel, [this](Ref*) {
        if (_undoClickCallback)
        {
            _undoClickCallback();
        }
    });
    undoItem->setPosition(Vec2(origin.x + visibleSize.width - 90.0f, origin.y + 60.0f));
    auto menu = Menu::create(undoItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    addChild(menu);
}

void GameView::_clearCardViews()
{
    for (CardView* cardView : _handCardViews)
    {
        cardView->removeFromParent();
    }
    for (CardView* cardView : _tableCardViews)
    {
        cardView->removeFromParent();
    }
    if (_topCardView != nullptr)
    {
        _topCardView->removeFromParent();
    }

    _handCardViews.clear();
    _tableCardViews.clear();
    _topCardView = nullptr;
    _cardViewById.clear();
}

void GameView::_rebuildCardViews(const GameModel& gameModel)
{

    auto topHandCard = gameModel.getTopHandCard();
    if (topHandCard != nullptr)
    {
        _topCardView = CardView::create();
        _topCardView->setCardModel(*topHandCard);
        _topCardView->setPosition(_topAnchor);
        addChild(_topCardView);
        _cardViewById[topHandCard->cardId] = _topCardView;
    }

    const auto& handCards = gameModel.getHandCards();
    _rebuildHandCardViews(handCards);
    
    const auto& tableCards = gameModel.getTableCards();
    _rebuildTableCardViews(tableCards);
}

void GameView::_rebuildHandCardViews(const std::vector<CardModel> &handCards)
{
    // 计算手牌的起始X坐标，使得所有手牌从右到左排列
    const float kHandDefaultGap = 120.0f;
    float kGap = handCards.empty() ? kHandDefaultGap : 540.0f / handCards.size();
    float handStartX = _handAnchor.x + kGap * (static_cast<float>(handCards.size()) - 1.0f) * 0.5f;
    for (int i = 0; i < static_cast<int>(handCards.size()); ++i)
    {
        CardView* cardView = CardView::create();
        cardView->setCardModel(handCards[i]);
        cardView->setPosition(Vec2(handStartX - i * kGap, _handAnchor.y));
        cardView->setClickCallback([this](int cardId) {
            if (_handCardClickCallback)
            {
                _handCardClickCallback(cardId);
            }
        });
        addChild(cardView);
        _handCardViews.push_back(cardView);
        _cardViewById[handCards[i].cardId] = cardView;
    }
}

void GameView::_rebuildTableCardViews(const std::vector<CardModel> &tableCards)
{
    const float kTableDefaultGap = 150.0f;
    float kGap = tableCards.empty() ? kTableDefaultGap : 1080.0f / tableCards.size();

    const float tableStartX = _tableAnchor.x - kGap * (static_cast<float>(tableCards.size()) - 1.0f) * 0.5f;
    for (int i = 0; i < static_cast<int>(tableCards.size()); ++i)
    {
        CardView* cardView = CardView::create();
        cardView->setCardModel(tableCards[i]);
        Vec2 cardPosition(tableStartX + i * kGap, _tableAnchor.y);
        if (tableCards[i].hasPresetPosition)
        {
            const auto origin = Director::getInstance()->getVisibleOrigin();
            cardPosition = Vec2(origin.x + tableCards[i].presetPosX, origin.y + tableCards[i].presetPosY);
        }
        cardView->setPosition(cardPosition);
        cardView->setClickCallback([this](int cardId) {
            if (_tableCardClickCallback)
            {
                _tableCardClickCallback(cardId);
            }
        });
        addChild(cardView);
        _tableCardViews.push_back(cardView);
        _cardViewById[tableCards[i].cardId] = cardView;
    }
}

void GameView::_playStackUndoAnimation(const UndoModel &undoModel, Vec2& targetPos)
{
    const auto& handCards = _gameModel->getHandCards();
        const int cardCountAfterUndo = static_cast<int>(handCards.size()) + 1;
        const float defaultGap = 120.0f;
        const float gap = cardCountAfterUndo > 0 ? 540.0f / cardCountAfterUndo : defaultGap;
        const float handStartX = _handAnchor.x + gap * (static_cast<float>(cardCountAfterUndo) - 1.0f) * 0.5f;

        int insertIndex = undoModel.sourceIndex;
        if (insertIndex < 0)
        {
            insertIndex = 0;
        }
        if (insertIndex >= cardCountAfterUndo)
        {
            insertIndex = cardCountAfterUndo - 1;
        }

        targetPos = Vec2(handStartX - static_cast<float>(insertIndex) * gap, _handAnchor.y);
}

void GameView::_playTableUndoAnimation(const UndoModel &undoModel, Vec2& targetPos)
{
    CardModel* topCard = _gameModel->getTopHandCard();
    if (topCard != nullptr && topCard->hasPresetPosition)
    {
        const auto origin = Director::getInstance()->getVisibleOrigin();
        targetPos = Vec2(origin.x + topCard->presetPosX, origin.y + topCard->presetPosY);
    }
    else
    {
        const auto& tableCards = _gameModel->getTableCards();
        const int cardCountAfterUndo = static_cast<int>(tableCards.size()) + 1;
        const float defaultGap = 150.0f;
        const float gap = cardCountAfterUndo > 0 ? 1080.0f / cardCountAfterUndo : defaultGap;
        const float tableStartX = _tableAnchor.x - gap *(static_cast<float>(cardCountAfterUndo) - 1.0f) * 0.5f;

        int insertIndex = undoModel.sourceIndex;
        if (insertIndex < 0)
        {
            insertIndex = 0;
        }
        if (insertIndex >= cardCountAfterUndo)
        {
            insertIndex = cardCountAfterUndo - 1;
        }

        targetPos = Vec2(tableStartX + static_cast<float>(insertIndex) * gap, _tableAnchor.y);
    }
}
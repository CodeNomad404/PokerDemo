#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include <string>
#include <vector>
#include <stack>

#include "models/CardModel.h"
#include "models/PlayfieldModel.h"
#include "models/StackModel.h"

class  GameModel
{
public:
    //getters
    int getLevelId() const; // 获取当前关卡ID
    CardModel* getCardById(int cardId) const; // 根据cardId获取CardModel

    // Hand / table are managed by dedicated sub-models, GameModel only forwards access.
    std::vector<CardModel>& getHandCards();
    const std::vector<CardModel>& getHandCards() const;
    std::vector<CardModel>& getTableCards();
    const std::vector<CardModel>& getTableCards() const;
    StackModel& getStackModel();
    const StackModel& getStackModel() const;
    PlayfieldModel& getPlayfieldModel();
    const PlayfieldModel& getPlayfieldModel() const;

    std::stack<CardModel>& getTopCards();
    const std::stack<CardModel>& getTopCards() const;

    CardModel* getTopHandCard() const; // 获取当前的手牌顶牌

    // setters
    void setHandCards(const std::vector<CardModel>& cards); // 设置手牌堆
    void setTableCards(const std::vector<CardModel>& cards); // 设置桌面牌堆
    void setTopHandCard(const CardModel& card); // 设置手牌顶牌
    void setLevelId(int id); // 设置当前关卡ID
    
    // 序列化和反序列化
    std::string serialize() const; // 将GameModel序列化为字符串
    static GameModel deserialize(const std::string& data); // 从字符串反序列化为GameModel
private:
    int levelId = 1;
    int handTopIndex = 0;
    
    std::vector<CardModel> _cards;
    StackModel _handCardsModel; // 手牌堆
    PlayfieldModel _tableCardsModel; // 桌面牌堆
    std::stack<CardModel> _topCards; // 顶牌堆
    CardModel _topHandCard; // 手牌顶牌
};

#endif

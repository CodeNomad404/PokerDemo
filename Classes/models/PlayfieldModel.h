#ifndef __PLAYFIELD_MODEL_H__
#define __PLAYFIELD_MODEL_H__

#include <vector>
#include <stack>
#include "models/CardModel.h"
class PlayfieldModel
{
public:

    // 获取桌面牌堆
    std::vector<CardModel>& getTableCards(); // 可读写
    const std::vector<CardModel>& getTableCards() const; // 只读

    // 设置桌面牌堆
    void setTableCards(const std::vector<CardModel> &cards);

    bool isEmpty() const; // 判断桌面牌堆是否为空

private:
    std::vector<CardModel> _tableCards; // 桌面牌堆
};

#endif // __PLAYFIELD_MODEL_H__
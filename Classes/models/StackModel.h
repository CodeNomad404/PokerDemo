#ifndef __STACK_MODEL_H__
#define __STACK_MODEL_H__

#include <vector>
#include <stack>
#include "CardModel.h"

class StackModel
{
public:
    // 获取手牌堆
    std::vector<CardModel>& getHandCards(); // 可读写
    const std::vector<CardModel>& getHandCards() const; // 只读

    void setHandCards(const std::vector<CardModel>& cards); // 设置手牌堆

    bool isEmpty() const; // 判断手牌堆是否为空
private:
    std::vector<CardModel> _handCards;
};

#endif // __STACK_MODEL_H__
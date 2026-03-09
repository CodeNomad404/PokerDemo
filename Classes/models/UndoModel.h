#ifndef __UNDO_MODEL_H__
#define __UNDO_MODEL_H__

#include <string>

#include "models/CardModel.h"

class UndoModel
{
public:
    CardArea sourceArea = CardArea::Unknown;
    int sourceIndex = -1;
private:
    std::string serialize() const; // 将UndoModel序列化为字符串
    static UndoModel deserialize(const std::string& data); // 从字符串反序列化为UndoModel
};

#endif

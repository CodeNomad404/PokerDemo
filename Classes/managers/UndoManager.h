#ifndef __UNDO_MANAGER_H__
#define __UNDO_MANAGER_H__

#include <vector>

#include "models/UndoModel.h"

class UndoManager
{
public:
    void init(); // 初始化撤销管理器状态
    void pushRecord(const UndoModel& undoModel); // 添加一个新的撤销记录
    bool hasUndoRecord() const; // 检查是否有可用的撤销记录
    UndoModel popRecord(); // 获取并移除最新的撤销记录
    void clearRecords(); // 清除所有撤销记录

private:
    std::vector<UndoModel> _undoRecords;
};

#endif

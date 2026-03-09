#include "managers/UndoManager.h"

void UndoManager::init()
{
    clearRecords();
}

void UndoManager::pushRecord(const UndoModel& undoModel)
{
    _undoRecords.push_back(undoModel);
}

bool UndoManager::hasUndoRecord() const
{
    return !_undoRecords.empty();
}

UndoModel UndoManager::popRecord()
{
    if (!hasUndoRecord())
    {
        return UndoModel{};
    }

    UndoModel undoModel = _undoRecords.back();
    _undoRecords.pop_back();
    return undoModel;
}

void UndoManager::clearRecords()
{
    _undoRecords.clear();
}

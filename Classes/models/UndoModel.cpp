#include "models/UndoModel.h"

#include <vector>

#include "utils/SerializeUtil.h"

std::string UndoModel::serialize() const
{
    return std::to_string(static_cast<int>(sourceArea)) + "," + std::to_string(sourceIndex);
}

UndoModel UndoModel::deserialize(const std::string& data)
{
    UndoModel undoModel;
    const std::vector<std::string> parts = SerializeUtil::split(data, ',');
    if (parts.size() < 2)
    {
        return undoModel;
    }

    undoModel.sourceArea = static_cast<CardArea>(std::stoi(parts[0]));
    undoModel.sourceIndex = std::stoi(parts[1]);
    return undoModel;
}

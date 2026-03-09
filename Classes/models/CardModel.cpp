#include "models/CardModel.h"

#include <vector>

#include "utils/SerializeUtil.h"

std::string CardModel::serialize() const
{
    return std::to_string(cardId) + "," +
           std::to_string(static_cast<int>(suit)) + "," +
           std::to_string(rank) + "," +
           std::to_string(static_cast<int>(area)) + "," +
           std::to_string(hasPresetPosition ? 1 : 0) + "," +
           std::to_string(presetPosX) + "," +
           std::to_string(presetPosY);
}

CardModel CardModel::deserialize(const std::string& data)
{
    CardModel cardModel;
    const std::vector<std::string> parts = SerializeUtil::split(data, ',');
    if (parts.size() < 5)
    {
        return cardModel;
    }

    cardModel.cardId = std::stoi(parts[0]);
    cardModel.suit = static_cast<CardSuitType>(std::stoi(parts[1]));
    cardModel.rank = static_cast<CardFaceType>(std::stoi(parts[2]));
    cardModel.area = static_cast<CardArea>(std::stoi(parts[4]));
    if (parts.size() >= 8)
    {
        cardModel.hasPresetPosition = std::stoi(parts[5]) != 0;
        cardModel.presetPosX = std::stof(parts[6]);
        cardModel.presetPosY = std::stof(parts[7]);
        cardModel.sourcePosX = cardModel.presetPosX;
        cardModel.sourcePosY = cardModel.presetPosY;
    }
    return cardModel;
}

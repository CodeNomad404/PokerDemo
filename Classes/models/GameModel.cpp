#include "models/GameModel.h"

#include <vector>

#include "utils/SerializeUtil.h"
#include "GameModel.h"

std::string GameModel::serialize() const
{
	std::vector<std::string> sections;
	sections.push_back(std::to_string(levelId));
	sections.push_back(std::to_string(handTopIndex));
	sections.push_back(_topHandCard.serialize());

	std::vector<std::string> handCardStrings;
	for (const CardModel& cardModel : _handCardsModel.getHandCards())
	{
		handCardStrings.push_back(cardModel.serialize());
	}
	sections.push_back(SerializeUtil::join(handCardStrings, ';'));

	std::vector<std::string> tableCardStrings;
	for (const CardModel& cardModel : _tableCardsModel.getTableCards())
	{
		tableCardStrings.push_back(cardModel.serialize());
	}
	sections.push_back(SerializeUtil::join(tableCardStrings, ';'));

	return SerializeUtil::join(sections, '|');
}

GameModel GameModel::deserialize(const std::string& data)
{
	GameModel gameModel;
	const std::vector<std::string> sections = SerializeUtil::split(data, '|');
	if (sections.size() < 5)
	{
		return gameModel;
	}

	gameModel.levelId = std::stoi(sections[0]);
	gameModel.handTopIndex = std::stoi(sections[1]);
	gameModel._topHandCard = CardModel::deserialize(sections[2]);

	const std::vector<std::string> handCardStrings = SerializeUtil::split(sections[3], ';');
	for (const std::string& handCardString : handCardStrings)
	{
		if (!handCardString.empty())
		{
			gameModel._handCardsModel.getHandCards().push_back(CardModel::deserialize(handCardString));
		}
	}

	const std::vector<std::string> tableCardStrings = SerializeUtil::split(sections[4], ';');
	for (const std::string& tableCardString : tableCardStrings)
	{
		if (!tableCardString.empty())
		{
			gameModel._tableCardsModel.getTableCards().push_back(CardModel::deserialize(tableCardString));
		}
	}

	return gameModel;
}

int GameModel::getLevelId() const
{
    return levelId;
}

CardModel *GameModel::getCardById(int cardId) const
{
	if (cardId <= 0 || static_cast<size_t>(cardId) >= _cards.size())
    	return nullptr;
    return const_cast<CardModel*>(&(_cards[cardId]));
}

std::vector<CardModel>& GameModel::getHandCards()
{
	return _handCardsModel.getHandCards();
}

const std::vector<CardModel>& GameModel::getHandCards() const
{
	return _handCardsModel.getHandCards();
}

std::vector<CardModel>& GameModel::getTableCards()
{
	return _tableCardsModel.getTableCards();
}

const std::vector<CardModel>& GameModel::getTableCards() const
{
	return _tableCardsModel.getTableCards();
}

StackModel& GameModel::getStackModel()
{
	return _handCardsModel;
}

const StackModel& GameModel::getStackModel() const
{
	return _handCardsModel;
}

PlayfieldModel& GameModel::getPlayfieldModel()
{
	return _tableCardsModel;
}

const PlayfieldModel& GameModel::getPlayfieldModel() const
{
	return _tableCardsModel;
}




std::stack<CardModel> &GameModel::getTopCards()
{
    return _topCards;
}

const std::stack<CardModel> &GameModel::getTopCards() const
{
	return _topCards;
}

CardModel *GameModel::getTopHandCard() const
{
    return _topHandCard.cardId != 0 ? const_cast<CardModel*>(&_topHandCard) : nullptr;
}

void GameModel::setTopHandCard(const CardModel &card)
{
	_topHandCard = card;
}

void GameModel::setHandCards(const std::vector<CardModel> &cards)
{
	_handCardsModel.setHandCards(cards);
}

void GameModel::setTableCards(const std::vector<CardModel> &cards)
{
	_tableCardsModel.setTableCards(cards);
}


void GameModel::setLevelId(int id)
{
	levelId = id;
}

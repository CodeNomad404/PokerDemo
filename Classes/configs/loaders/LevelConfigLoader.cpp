#include "configs/loaders/LevelConfigLoader.h"
#include "configs/models/CardResConfig.h"
#include "cocos2d.h"
#include "json/document.h"
#include "models/CardModel.h"
#include "models/GameModel.h"

using namespace cocos2d;

namespace {
const CardResConfig kCardResConfig;

bool isRedSuit(CardSuitType suit) {
    return suit == CardSuitType::CST_DIAMONDS || suit == CardSuitType::CST_HEARTS;
}

std::string rankToToken(CardFaceType rank) {
    switch (rank) {
    case CardFaceType::CFT_ACE: return "A";
    case CardFaceType::CFT_TWO: return "2";
    case CardFaceType::CFT_THREE: return "3";
    case CardFaceType::CFT_FOUR: return "4";
    case CardFaceType::CFT_FIVE: return "5";
    case CardFaceType::CFT_SIX: return "6";
    case CardFaceType::CFT_SEVEN: return "7";
    case CardFaceType::CFT_EIGHT: return "8";
    case CardFaceType::CFT_NINE: return "9";
    case CardFaceType::CFT_TEN: return "10";
    case CardFaceType::CFT_JACK: return "J";
    case CardFaceType::CFT_QUEEN: return "Q";
    case CardFaceType::CFT_KING: return "K";
    default: return "A";
    }
}

std::string suitToToken(CardSuitType suit) {
    switch (suit) {
    case CardSuitType::CST_CLUBS: return "club";
    case CardSuitType::CST_DIAMONDS: return "diamond";
    case CardSuitType::CST_HEARTS: return "heart";
    case CardSuitType::CST_SPADES: return "spade";
    default: return "spade";
    }
}

bool tryParseCard(const rapidjson::Value& v, int cardId, CardArea area, CardModel& out) {
    if (!v.IsObject()) return false;
    if (!v.HasMember("CardFace") || !v["CardFace"].IsInt()) return false;
    if (!v.HasMember("CardSuit") || !v["CardSuit"].IsInt()) return false;

    out.cardId = cardId;
    out.area = area;
    out.rank = static_cast<CardFaceType>(v["CardFace"].GetInt());
    out.suit = static_cast<CardSuitType>(v["CardSuit"].GetInt());

    const std::string color = isRedSuit(out.suit) ? "red" : "black";
    const std::string rank = rankToToken(out.rank);
    out.suitPath = kCardResConfig.buildSuitPath(suitToToken(out.suit));
    out.bigRankPath = kCardResConfig.buildBigNumberPath(color, rank);
    out.smallRankPath = kCardResConfig.buildSmallNumberPath(color, rank);

    if (area == CardArea::Playfield && v.HasMember("Position") && v["Position"].IsObject()) {
        const auto& pos = v["Position"];
        if (pos.HasMember("x") && pos.HasMember("y") && pos["x"].IsNumber() && pos["y"].IsNumber()) {
            out.hasPresetPosition = true;
            out.presetPosX = pos["x"].GetFloat();
            out.presetPosY = pos["y"].GetFloat()+580;
        }
    }

    return true;
}
}

LevelConfig LevelConfigLoader::load(int levelId) const {
    LevelConfig cfg;
    cfg.levelId = levelId;

    // 目前简单地从一个JSON文件中加载所有关卡配置，实际项目中可以根据levelId加载不同的文件或不同的JSON
    const std::string jsonText = FileUtils::getInstance()->getStringFromFile("configs/Levels.json");
    if (jsonText.empty()) {
        return cfg;
    }

    rapidjson::Document doc;
    doc.Parse(jsonText.c_str());
    if (doc.HasParseError() || !doc.IsObject()) {
        return cfg;
    }

    int nextId = 1;
    if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
        for (const auto& item : doc["Stack"].GetArray()) {
            CardModel c;
            if (tryParseCard(item, nextId, CardArea::Stack, c)) {
                cfg.stackCards.push_back(c);
                ++nextId;
            }
        }
    }

    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
        for (const auto& item : doc["Playfield"].GetArray()) {
            CardModel c;
            if (tryParseCard(item, nextId, CardArea::Playfield, c)) {
                cfg.playfieldCards.push_back(c);
                ++nextId;
            }
        }
    }

    return cfg;
}

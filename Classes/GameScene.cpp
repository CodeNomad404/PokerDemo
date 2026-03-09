#include "GameScene.h"

#include "controllers/GameController.h"
#include "models/GameModel.h"
#include "views/GameView.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    return GameScene::create();
}

GameScene::~GameScene() = default;

bool GameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    _gameModel = std::make_unique<GameModel>();
    _gameView = GameView::create(); 
    if (_gameView == nullptr)
    {
        return false;
    }

    _gameController = std::make_unique<GameController>(_gameModel.get(), _gameView);

    addChild(_gameView);

    _gameController->startGame(1);

    return true;
}

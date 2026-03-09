#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include <memory>

#include "cocos2d.h"

#include "controllers/GameController.h"
#include "models/GameModel.h"
#include "views/GameView.h"

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    ~GameScene() override;

    bool init() override;

    CREATE_FUNC(GameScene);

private:
    std::unique_ptr<GameModel> _gameModel;
    std::unique_ptr<GameController> _gameController;
    GameView* _gameView;
};

#endif

//
//  GameScene.h
//  Tetrominos
//
//  Created by ryutamaki on 2015/07/01.
//
//

#ifndef __Tetrominos__GameScene__
#define __Tetrominos__GameScene__

#include "cocos2d.h"
#include "CocosGUI.h"

#include "Constants.h"

#include "TetrominoBag.h"

class Grid;
class Tetromino;
class Coordinate;

class GameScene : public cocos2d::Node {
public:
    CREATE_FUNC(GameScene);

private:
    Grid* grid;
    std::unique_ptr<TetrominoBag> tetrominoBag;
    bool active;
    int totalScore;
    cocos2d::ui::Text* scoreLabel;
    float stepInterval;

    // View lifecycle
    bool init() override;
    void onEnter() override;
    void setupTouchHandling();

    // Game Logic
    Tetromino* createRandomTetromino();
    void setGameActive(bool active);
    void step(float dt);
    void updateStateFromScore();
    void updateGameSpeed(int score);

    // Utility
    Coordinate convertPositionToCoordinate(cocos2d::Vec2 position);

    // UI
    void backButtonPressed(Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    void updateScoreLabel(int score);
};

#endif /* defined(__Tetrominos__GameScene__) */

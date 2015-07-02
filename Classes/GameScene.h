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

#include "TetrominoBag.h"

class Grid;
class Tetromino;

class GameScene : public cocos2d::Node {
public:
    CREATE_FUNC(GameScene);

private:
    Grid* grid;
    std::unique_ptr<TetrominoBag> tetrominoBag;

    bool active;

    bool init() override;
    void onEnter() override;

    void setupTouchHandling();

    Tetromino* createRandomTetromino();

    void backButtonPressed(Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);

    void setGameActive(bool active);

    void step(float dt);

};

#endif /* defined(__Tetrominos__GameScene__) */

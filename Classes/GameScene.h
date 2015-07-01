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

class Grid;

class GameScene : public cocos2d::Node {
public:
    CREATE_FUNC(GameScene);

private:
    Grid* grid;

    bool init() override;
    void onEnter() override;

    void backButtonPressed(Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);

};

#endif /* defined(__Tetrominos__GameScene__) */

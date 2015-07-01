//
//  GameScene.cpp
//  Tetrominos
//
//  Created by ryutamaki on 2015/07/01.
//
//

#include "GameScene.h"

using namespace cocos2d;

bool GameScene::init() {
    if (!Node::init()) {
        return false;
    }

    LayerColor* background = LayerColor::create(Color4B(255, 255, 255, 255));
    this->addChild(background);

    return true;
}

//
//  Lobby.cpp
//  Tetrominos
//
//  Created by ryutamaki on 2015/07/01.
//
//

#include "Lobby.h"
#include "SceneManager.h"

using namespace cocos2d;

bool Lobby::init() {
    if (!Node::init()) {
        return false;
    }

    LayerColor* background = LayerColor::create(Color4B(255, 255, 255, 255));
    this->addChild(background);

    return true;
}

#pragma mark - Node lifecycle

void Lobby::onEnter() {
    Node::onEnter();

    Size visibleSize = Director::getInstance()->getVisibleSize();

    Sprite* logoSprite = Sprite::create("logo.png");
    logoSprite->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.8f));
    this->addChild(logoSprite);

    ui::Button* singlePlayerButton = ui::Button::create();
    singlePlayerButton->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.4f));
    singlePlayerButton->loadTextures("singlePlayerButton.png", "singlePlayerButtonPressed.png");
    singlePlayerButton->addTouchEventListener(CC_CALLBACK_2(Lobby::singlePlayerPressed, this));
    this->addChild(singlePlayerButton);

    ui::Button* multiplayerButton = ui::Button::create();
    multiplayerButton->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.25f));
    multiplayerButton->loadTextures("multiplayerButton.png", "multiplayerButtonPressed.png");
    multiplayerButton->addTouchEventListener(CC_CALLBACK_2(Lobby::multiplayerPressed, this));
    this->addChild(multiplayerButton);
}

#pragma mark - Private methods

void Lobby::singlePlayerPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType eEventType) {
    if (eEventType == ui::Widget::TouchEventType::ENDED) {
        SceneManager::getInstance()->enterGameScene(false);
    }
}

void Lobby::multiplayerPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType eEventType) {
    CCLOG("2 player!");
}
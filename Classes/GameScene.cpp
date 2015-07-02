//
//  GameScene.cpp
//  Tetrominos
//
//  Created by ryutamaki on 2015/07/01.
//
//

#include "GameScene.h"
#include "SceneManager.h"

#include "Grid.h"
#include "Tetromino.h"

using namespace cocos2d;

bool GameScene::init()
{
    if (!Node::init())
    {
        return false;
    }

    LayerColor* background = LayerColor::create(Color4B(255, 255, 255, 255));
    this->addChild(background);

    this->tetrominoBag = std::unique_ptr<TetrominoBag>(new TetrominoBag());

    return true;
}

#pragma mark - View lifecycle

void GameScene::onEnter()
{
    Node::onEnter();

    Size visibleSize = Director::getInstance()->getVisibleSize();

    // serup grid
    this->grid = Grid::create();
    this->grid->setAnchorPoint(Vec2(0.5f, 0.0f));
    this->grid->setPosition(Vec2(visibleSize.width * 0.5f, 0.0f));
    this->addChild(this->grid);

    Tetromino* randomTest = this->createRandomTetromino();
    this->grid->spawnTetromino(randomTest);

    // setup menus
    ui::Button* backButton = ui::Button::create();
    backButton->setAnchorPoint(Vec2(0.0f, 1.0f));
    backButton->setPosition(Vec2(0.0f, visibleSize.height));
    backButton->loadTextures("backButton.png", "backButtonPressed.png");
    backButton->addTouchEventListener(CC_CALLBACK_2(GameScene::backButtonPressed, this));
    this->addChild(backButton);

    this->setupTouchHandling();
}

void GameScene::setupTouchHandling()
{
    EventListenerTouchOneByOne* touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        return true;
    };
    touchListener->onTouchEnded = [&](Touch* touch, Event* event)
    {
        this->grid->rotateActiveTetromino();
    };

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

#pragma mark - Public methods

#pragma mark - Private methods

Tetromino* GameScene::createRandomTetromino()
{
    TetrominoType tetrominoType = this->tetrominoBag->getTetrominoType();
    Tetromino* newTetromino = Tetromino::createWithType(tetrominoType);
    return newTetromino;
}

#pragma mark - Callbacks

void GameScene::backButtonPressed(cocos2d::Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    if (eEventType == ui::Widget::TouchEventType::ENDED)
    {
        SceneManager::getInstance()->exitGameScene();
    }
}

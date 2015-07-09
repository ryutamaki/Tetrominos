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

#include <time.h>

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

    this->active = false;

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

    this->setGameActive(true);
}

void GameScene::setupTouchHandling()
{
    EventListenerTouchOneByOne* touchListener = EventListenerTouchOneByOne::create();

    static Vec2 firstTouchPos;
    static Vec2 lastTouchPos;
    static bool allowRotate;
    static std::clock_t touchStartedTime;

    touchListener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        firstTouchPos = this->convertTouchToNodeSpace(touch);
        lastTouchPos = firstTouchPos;
        allowRotate = true;
        touchStartedTime = clock();//std::clock();
        return true;
    };
    touchListener->onTouchMoved = [&](Touch* touch, Event* event)
    {
        Vec2 touchPos = this->convertTouchToNodeSpace(touch);

        Vec2 difference = touchPos - lastTouchPos;

        Tetromino* activeTetromino = this->grid->getActiveTetromino();
        if (!activeTetromino)
        {
            return;
        }

        Coordinate touchCoordinate = this->convertPositionToCoordinate(touchPos);
        Coordinate differenceCoordinate = this->convertPositionToCoordinate(difference);
        Coordinate activeTetrominoCoordinate = this->grid->getActiveTetrominoCoordinate();

        if (differenceCoordinate.y <= -1)
        {
            Coordinate newTetrominoCoordinate = Coordinate(activeTetrominoCoordinate.x, activeTetrominoCoordinate.y - 1);
            this->grid->setActiveTetrominoCoordinate(newTetrominoCoordinate);

            lastTouchPos = touchPos;
        }
        else if (abs(differenceCoordinate.x) >= 1)
        {
            bool movingRight = differenceCoordinate.x > 0;
            Coordinate newTetrominoCoordinate;
            newTetrominoCoordinate = Coordinate(activeTetrominoCoordinate.x + (movingRight ? 1 : -1), activeTetrominoCoordinate.y);
            this->grid->setActiveTetrominoCoordinate(newTetrominoCoordinate);
            allowRotate = false;

            lastTouchPos = touchPos;
        }
    };
    touchListener->onTouchEnded = [&](Touch* touch, Event* event)
    {
        Vec2 touchEndPos = this->convertTouchToNodeSpace(touch);

        Vec2 difference = touchEndPos - firstTouchPos; // maybe use lastTouchPos
        float touchDuration = (float)(clock() - touchStartedTime) / CLOCKS_PER_SEC;
        float touchVelocity = fabsf(difference.y / touchDuration);

        log("%f", touchVelocity);
        if (touchVelocity > DROP_VELOCITY)
        {
            log("DROP TETROMINO");
        }
        else
        {
            float distance = touchEndPos.distance(firstTouchPos);
            Size blocksize = this->grid->getBlockSize();

            if (distance < blocksize.width && allowRotate)
            {
                this->grid->rotateActiveTetromino();
            }
        }
    };

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

#pragma mark - Public methods

Coordinate GameScene::convertPositionToCoordinate(Vec2 position)
{
    Size contentSize = this->getContentSize();
    Size blockSize = this->grid->getBlockSize();

    int coordinateX = position.x / blockSize.width;
    int coordinateY = position.y / blockSize.height;

    return Coordinate(coordinateX, coordinateY);
}

#pragma mark - Private methods

Tetromino* GameScene::createRandomTetromino()
{
    TetrominoType tetrominoType = this->tetrominoBag->getTetrominoType();
    Tetromino* newTetromino = Tetromino::createWithType(tetrominoType);
    return newTetromino;
}

void GameScene::setGameActive(bool active)
{
    this->active = active;
    if (active)
    {
        this->schedule(CC_SCHEDULE_SELECTOR(GameScene::step), INITIAL_STEP_INTERVAL);
    }
    else
    {
        this->unschedule(CC_SCHEDULE_SELECTOR(GameScene::step));
    }
}

void GameScene::step(float dt)
{
    this->grid->step();
}

#pragma mark - Callbacks

void GameScene::backButtonPressed(cocos2d::Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    if (eEventType == ui::Widget::TouchEventType::ENDED)
    {
        SceneManager::getInstance()->exitGameScene();
    }
}

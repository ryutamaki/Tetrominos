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

#include "UIConstants.h"

#include <time.h>
#include "CocosGUI.h"

using namespace cocos2d;

bool GameScene::init()
{
    if (!Node::init())
    {
        return false;
    }

    LayerColor* background = LayerColor::create(Color4B(255, 255, 255, 255));
    this->addChild(background);

    this->stepInterval = INITIAL_STEP_INTERVAL;
    this->tetrominoBag = std::unique_ptr<TetrominoBag>(new TetrominoBag());
    this->totalScore = 0;

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

    // setup menus
    ui::Button* backButton = ui::Button::create();
    backButton->setAnchorPoint(Vec2(0.0f, 1.0f));
    backButton->setPosition(Vec2(0.0f, visibleSize.height));
    backButton->loadTextures("backButton.png", "backButtonPressed.png");
    backButton->addTouchEventListener(CC_CALLBACK_2(GameScene::backButtonPressed, this));
    this->addChild(backButton);

    // setup labels
    this->scoreLabel = ui::Text::create("0", FONT_NAME, FONT_SIZE);
    this->scoreLabel->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.95f));
    this->setAnchorPoint(Vec2(0.5f, 1.0f));
    this->scoreLabel->setColor(LABEL_COLOR);
    this->addChild(this->scoreLabel);

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
        if (!this->grid->getActiveTetromino())
        {
            return;
        }
        Vec2 touchEndPos = this->convertTouchToNodeSpace(touch);

        Vec2 difference = touchEndPos - firstTouchPos; // maybe use lastTouchPos
        float touchDuration = (float)(clock() - touchStartedTime) / CLOCKS_PER_SEC;
        float touchVelocity = fabsf(difference.y / touchDuration);

        log("%f", touchVelocity);
        if (touchVelocity > DROP_VELOCITY)
        {
            this->grid->dropActiveTetromino();
            this->updateStateFromScore();
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
        this->schedule(CC_SCHEDULE_SELECTOR(GameScene::step), this->stepInterval);
    }
    else
    {
        this->unschedule(CC_SCHEDULE_SELECTOR(GameScene::step));
    }
}

void GameScene::step(float dt)
{
    if (!this->grid->getActiveTetromino())
    {
        Tetromino* randomTest = this->createRandomTetromino();
        this->grid->spawnTetromino(randomTest);
        return;
    }

    this->grid->step();
    this->updateStateFromScore();
}

void GameScene::updateStateFromScore()
{
    int newScore = this->grid->getScore();
    if (newScore > this->totalScore)
    {
        this->totalScore = newScore;
        this->updateScoreLabel(newScore);
        this->updateGameSpeed(this->totalScore);
    }
}

void GameScene::updateGameSpeed(int score)
{
    int stepAcceleration = score / SCORE_TO_ACCELERATE;
    float intervalDeduction = powf(ACCELERATION_FACTOR, stepAcceleration);
//    float intervalDeduction = INITIAL_STEP_INTERVAL * float(stepAcceleration) * ACCELERATION_FACTOR;
    float newInterval = MAX((INITIAL_STEP_INTERVAL * intervalDeduction), SPEED_MAX);
    this->stepInterval = newInterval;
    this->unschedule(CC_SCHEDULE_SELECTOR(GameScene::step));
    this->schedule(CC_SCHEDULE_SELECTOR(GameScene::step), this->stepInterval);
}

#pragma mark - UI

void GameScene::backButtonPressed(cocos2d::Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    if (eEventType == ui::Widget::TouchEventType::ENDED)
    {
        SceneManager::getInstance()->exitGameScene();
    }
}

void GameScene::updateScoreLabel(int score)
{
    this->scoreLabel->setString(StringUtils::toString(score));
}
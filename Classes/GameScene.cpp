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
#include "JSONPacker.h"
#include "NetworkingWrapper.h"

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
    this->timeLeft = TIME_PER_GAME;
    this->networkedSession = false;
    this->isGameOver = false;

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
    this->scoreLabel->setAnchorPoint(Vec2(0.5f, 1.0f));
    this->scoreLabel->setColor(LABEL_COLOR);
    this->addChild(this->scoreLabel);

    this->timeLeftLabel = ui::Text::create("0", FONT_NAME, FONT_SIZE);
    this->timeLeftLabel->setPosition(this->scoreLabel->getPosition() - Vec2(0.0f, FONT_SIZE * 1.5f));
    this->timeLeftLabel->setAnchorPoint(Vec2(0.5f, 1.0f));
    this->timeLeftLabel->setColor(LABEL_COLOR);
    this->addChild(this->timeLeftLabel);

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

void GameScene::setNetworkedSession(bool networkedSession)
{
    this->networkedSession = networkedSession;
}

void GameScene::receivedData(const void *data, unsigned long length)
{
    const char* cstr = reinterpret_cast<const char*>(data);
    std::string json = std::string(cstr, length);
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
        this->scheduleUpdate();
    }
    else
    {
        this->unschedule(CC_SCHEDULE_SELECTOR(GameScene::step));
        this->unscheduleUpdate();
    }
}

void GameScene::step(float dt)
{
    if (this->grid->checkIfTopReached())
    {
        this->gameOver();
    }

    if (!this->grid->getActiveTetromino())
    {
        Tetromino* randomTest = this->createRandomTetromino();
        this->grid->spawnTetromino(randomTest);
    }
    else
    {
        this->grid->step();
        this->updateStateFromScore();
    }

    if (this->networkedSession)
    {
        this->sendGameStateOverNetwork();
    }
}

void GameScene::update(float dt)
{
    Node::update(dt);

    this->setTimeLeft(this->timeLeft - dt);

    if (this->timeLeft <= 0.0f)
    {
        this->gameOver();
    }
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

void GameScene::gameOver()
{
    this->isGameOver = true;
    this->setGameActive(false);

    if (this->networkedSession)
    {
        this->sendGameStateOverNetwork();
    }

    std::string scoreString = StringUtils::toString(totalScore);
    std::string messageContent = "Your score is " + scoreString + "!";

    MessageBox(messageContent.c_str(), "Game Over");

    SceneManager::getInstance()->exitGameScene();
}

void GameScene::setTimeLeft(float time)
{
    this->timeLeft = time;
    std::string timeLeftString = StringUtils::format("%2.1f", time);
    this->timeLeftLabel->setString(StringUtils::toString(timeLeftString));
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

#pragma mark - Networking

void GameScene::sendGameStateOverNetwork()
{
    JSONPacker::GameState state;

    state.name = NetworkingWrapper::getDeviceName();
    state.score = this->totalScore;
    state.gameOver = this->isGameOver;

    std::vector<std::vector<Sprite*>> blocksLanded = this->grid->getBlocksLanded();

    for (int y = 0; y < blocksLanded.size(); y++)
    {
        std::vector<Color3B> blocks(blocksLanded.size(), Color3B::WHITE);
        state.board.push_back(blocks);

        std::vector<Sprite*> column = blocksLanded[y];

        for (int x = 0; x < column.size(); x++)
        {
            Sprite* block = column[x];
            if (block)
            {
                state.board[y][x] = block->getColor();
            }
        }
    }

    Tetromino* activeTetromino = this->grid->getActiveTetromino();
    if (activeTetromino)
    {
        std::vector<Coordinate> coordinates = activeTetromino->getCurrentRotation();
        Coordinate tetrominoCoordinate = this->grid->getActiveTetrominoCoordinate();
        Color3B color = activeTetromino->getTetrominoColor();

        for (Coordinate blockCoordinate : coordinates)
        {
            Coordinate gridCoordinate = Coordinate::add(tetrominoCoordinate, blockCoordinate);
            state.board[gridCoordinate.y][gridCoordinate.x] = color;
        }
    }

    std::string json = JSONPacker::packGameState(state);
    SceneManager::getInstance()->sendData(json.c_str(), json.length());
}

#pragma mark - Utility

Coordinate GameScene::convertPositionToCoordinate(Vec2 position)
{
    Size contentSize = this->getContentSize();
    Size blockSize = this->grid->getBlockSize();

    int coordinateX = position.x / blockSize.width;
    int coordinateY = position.y / blockSize.height;

    return Coordinate(coordinateX, coordinateY);
}

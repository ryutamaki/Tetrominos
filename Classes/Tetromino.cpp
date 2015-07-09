//
//  Tetromino.cpp
//  Tetrominos
//
//  Created by ryutamaki on 2015/07/01.
//
//

#include "Tetromino.h"
#include "JSONPacker.h"

USING_NS_CC;

#define GRID_SIZE 4

#pragma mark - Initializing methods

Tetromino* Tetromino::createWithType(TetrominoType type)
{
    Tetromino* tetromino = new (std::nothrow) Tetromino();

    if (tetromino && tetromino->initWithType(type))
    {
        tetromino->autorelease();
        return tetromino;
    }
    CC_SAFE_DELETE(tetromino);
    return nullptr;
}

bool Tetromino::initWithType(TetrominoType type)
{
    if (!Node::init())
    {
        return false;
    }
    this->type = type;
    this->rotations = std::vector<std::vector<Coordinate>>();

    // How to load json file using FileUtils
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename("tetrominos.json");
    std::string jsonString = FileUtils::getInstance()->getStringFromFile(fullPath);

    JSONPacker::TetrominoState tetrominoStete = JSONPacker::unpackTetrominoJSON(jsonString, type);

    this->rotations = tetrominoStete.rotations;
    this->color = tetrominoStete.color;
    this->blocks = std::vector<Sprite*>();
    this->blocks.reserve(4);
    this->rotationIndex = 0;

    Sprite* dummyBlock = Sprite::create("block.png");
    Size dummySize = dummyBlock->getContentSize();

    float gridSizeFloat = float(GRID_SIZE);
    this->setContentSize(Size(dummySize.width * gridSizeFloat, dummySize.height * gridSizeFloat));

    auto coordinates = rotations[this->rotationIndex];
    for (Coordinate coordinate : coordinates)
    {
        Sprite* block = Sprite::create("block.png");
        block->setColor(tetrominoStete.color);
        block->setAnchorPoint(Vec2(0.0f, 0.0f));
        block->setPosition(Vec2(coordinate.x * dummySize.width, coordinate.y * dummySize.height));
        this->addChild(block);
        this->blocks.push_back(block);
    }

    return true;
}

#pragma mark - View lifecycle

void Tetromino::onEnter()
{
    Node::onEnter();
}

void Tetromino::onExit()
{
    Node::onExit();
}

#pragma mark - Public methods

void Tetromino::rotate(bool right)
{
    if (right)
    {
        this->rotationIndex++;
    }
    else
    {
        this->rotationIndex--;
    }

    if (this->rotationIndex < 0)
    {
        this->rotationIndex = (int)rotations.size() - 1;
    }
    else if (this->rotations.size() <= this->rotationIndex)
    {
        this->rotationIndex = 0;
    }

    auto coordinates = rotations[this->rotationIndex];

    for (int index = 0; index < GRID_SIZE; index++)
    {
        Sprite* block = blocks[index];
        Coordinate coordinate = coordinates[index];

        Size blockSize = block->getContentSize();
        block->setPosition(Vec2(coordinate.x * blockSize.width, coordinate.y * blockSize.height));
    }
}

#pragma mark - Accessor

int Tetromino::getHighestYCoordinate()
{
    std::vector<Coordinate> coordinates = this->rotations[this->rotationIndex];

    int maxY = 0;
    for (Coordinate coordinate : coordinates)
    {
        maxY = MAX(coordinate.y, maxY);
    }
    return maxY;
}

int Tetromino::getMinimunXCoordinate()
{
    std::vector<Coordinate> coordinates = this->rotations[this->rotationIndex];

    int minX = GRID_SIZE - 1;
    for (Coordinate coordinate : coordinates)
    {
        minX = MIN(coordinate.x, minX);
    }
    return minX;
}

int Tetromino::getWidthInBlocks()
{
    std::vector<Coordinate> coordinates = this->rotations[this->rotationIndex];
    int minX = GRID_SIZE - 1;
    int maxX = 0;
    for (Coordinate coordinate : coordinates)
    {
        minX = MIN(coordinate.x, minX);
        maxX = MAX(coordinate.x, maxX);
    }
    return maxX - minX + 1;
}

std::vector<int> Tetromino::getSkirt()
{
    int minX = this->getMinimunXCoordinate();
    std::vector<int> skirtVector = std::vector<int>(this->getWidthInBlocks(), GRID_SIZE - 1);

    std::vector<Coordinate> coordinates = this->rotations[this->rotationIndex];

    for (Coordinate coordinate : coordinates)
    {
        int skirtVectorIndex = coordinate.x - minX;
        skirtVector[skirtVectorIndex] = MIN(skirtVector[skirtVectorIndex], coordinate.y);
    }
    return skirtVector;
}
//
//  Grid.cpp
//  Tetrominos
//
//  Created by ryutamaki on 2015/07/01.
//
//

#include "Grid.h"
#include "Tetromino.h"

USING_NS_CC;

#pragma mark - Initializing methods

bool Grid::init() {
    if (!Sprite::initWithFile("grid.png")) {
        return false;
    }

    this->activeTetromino = nullptr;
    this->activeTetrominoCoordinate = Coordinate();

    for (int rowNumber = 0; rowNumber < GRID_HEIGHT; rowNumber++)
    {
        std::vector<Sprite*> row(GRID_WIDTH, nullptr);
        this->blocksLanded.push_back(row);
    }

    return true;
}

#pragma mark - View lifecycle

void Grid::onEnter()
{
    Sprite::onEnter();
}

#pragma mark - Public methods

void Grid::spawnTetromino(Tetromino *tetromino)
{
    this->activeTetromino = tetromino;

    // TODO: Plase a tetromino in correct position in grid
    this->activeTetromino->setAnchorPoint(Vec2(0.0f, 0.0f));

    int highestY = this->activeTetromino->getHighestYCoordinate();
    int width = this->activeTetromino->getWidthInBlocks();

    Coordinate spawnCoordinate = Coordinate((GRID_WIDTH / 2) - (width / 2) - 1, GRID_HEIGHT - highestY - 1);

    this->setActiveTetrominoCoordinate(spawnCoordinate);

    this->addChild(this->activeTetromino);
}

void Grid::rotateActiveTetromino()
{
    if (!this->activeTetromino)
    {
        return;
    }

    this->activeTetromino->rotate(true);

    if (this->checkIfTetrominoCollides(this->activeTetromino, this->activeTetrominoCoordinate))
    {
        this->activeTetromino->rotate(false);
    }

    // TODO: check if collision, undo rotation
}

void Grid::step()
{
    if (this->activeTetromino)
    {
        Coordinate activeCoordinate = this->getActiveTetrominoCoordinate();
        Coordinate nextCoordinate = Coordinate(activeCoordinate.x, activeCoordinate.y - 1);

        if (this->checkIfTetrominoCollides(this->activeTetromino, nextCoordinate))
        {
            this->deactivateTetromino(this->activeTetromino, activeCoordinate);
        }
        else
        {
            this->setActiveTetrominoCoordinate(nextCoordinate);
        }
    }
}

#pragma mark - Accessor

void Grid::setActiveTetrominoCoordinate(Coordinate coordinate)
{
    if (this->activeTetromino)
    {
        if (this->checkIfTetrominoCollides(this->activeTetromino, coordinate))
        {
            return;
        }

        this->activeTetrominoCoordinate = coordinate;
        this->activeTetromino->setPosition(this->convertCoordinateToPosition(coordinate));
    }
}

Coordinate Grid::getActiveTetrominoCoordinate()
{
    return this->activeTetrominoCoordinate;
}

Size Grid::getBlockSize()
{
    Size contentSize = this->getContentSize();

    float blockWidth = contentSize.width / float(GRID_WIDTH);
    float blockHeight = contentSize.height / float(GRID_HEIGHT);
    return Size(blockWidth, blockHeight);
}

Tetromino* Grid::getActiveTetromino()
{
    return this->activeTetromino;
}

#pragma mark - Private methods

Vec2 Grid::convertCoordinateToPosition(Coordinate coordinate)
{
    Size contentSize = this->getContentSize();

    float blockWidth = contentSize.width / float(GRID_WIDTH);
    float blockHeight = contentSize.height / float(GRID_HEIGHT);

    return Vec2(coordinate.x * blockWidth, coordinate.y * blockHeight);
}

bool Grid::checkIfTetrominoCollides(Tetromino *tetromino, Coordinate tetrominoCoordinate)
{
    int skirtStart = tetromino->getMinimunXCoordinate();
    std::vector<int> skirt = tetromino->getSkirt();

    for (int index = 0; index < skirt.size(); index++)
    {
        Coordinate localCoordinate = Coordinate(index + skirtStart, skirt[index]);
        Coordinate gridCoordinate = Coordinate::add(tetrominoCoordinate, localCoordinate);

        if (gridCoordinate.x < 0 || gridCoordinate.y < 0 || gridCoordinate.x >= GRID_WIDTH || gridCoordinate.y >= GRID_HEIGHT)
        {
            return true;
        }

        if (blocksLanded[gridCoordinate.y][gridCoordinate.x])
        {
            return true;;
        }
    }
    return false;
}

void Grid::deactivateTetromino(Tetromino *tetromino, Coordinate tetrominoCoordinate)
{
    this->placeTetrominoOnboard(tetromino, tetrominoCoordinate);

    this->activeTetromino->removeFromParent();
    this->activeTetromino = nullptr;
}

void Grid::placeTetrominoOnboard(Tetromino *tetromino, Coordinate tetrominoCoordinate)
{
    std::vector<Sprite*> blocks = tetromino->getBlocks();
    std::vector<Coordinate> rotation = tetromino->getCurrentRotation();

    for (int index = 0; index < BLOCKS_PER_TETROMINO; index++)
    {
        // get tetromino coordinate
        // convert to grid coordinate
        Coordinate localCoordinate = rotation[index];
        Coordinate globalCoordinate = Coordinate::add(tetrominoCoordinate, localCoordinate);
        
        // get the block sprite
        Sprite* block = blocks[index];
        Vec2 gridPosition = block->getPosition() + tetromino->getPosition();

        // remove the block from the tetromino
        block->retain();
        block->removeFromParent();
        // add the block to grid (this)
        this->addChild(block);
        block->release();
        // set the block position
        block->setPosition(gridPosition);
        // add the block to blocksLanded
        this->blocksLanded[globalCoordinate.y][globalCoordinate.x] = block;
    }
}
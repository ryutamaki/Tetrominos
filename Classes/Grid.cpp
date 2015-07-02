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
    if (this->activeTetromino)
    {
        this->activeTetromino->rotate(true);
    }

    // TODO: check if collision, undo rotation
}

void Grid::step()
{
    Coordinate activeCoordinate = this->getActiveTetrominoCoordinate();
    Coordinate nextCoordinate = Coordinate(activeCoordinate.x, activeCoordinate.y - 1);
    this->setActiveTetrominoCoordinate(nextCoordinate);
}

#pragma mark - Accessor

void Grid::setActiveTetrominoCoordinate(Coordinate coordinate)
{
    if (this->activeTetromino)
    {
        this->activeTetrominoCoordinate = coordinate;

        this->activeTetromino->setPosition(this->convertCoordinatetoPosition(coordinate));
    }
}

Coordinate Grid::getActiveTetrominoCoordinate()
{
    return this->activeTetrominoCoordinate;
}

#pragma mark - Private methods

Vec2 Grid::convertCoordinatetoPosition(Coordinate coordinate)
{
    Size contentSize = this->getContentSize();

    float blockWidth = contentSize.width / float(GRID_WIDTH);
    float blockHeight = contentSize.height / float(GRID_HEIGHT);

    return Vec2(coordinate.x * blockWidth, coordinate.y * blockHeight);
}

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

    return true;
}

#pragma mark - View lifecycle

void Grid::onEnter()
{
    Sprite::onEnter();

    this->activeTetromino = Tetromino::createWithType(TetrominoType::T);
    this->addChild(this->activeTetromino);

    this->schedule(CC_SCHEDULE_SELECTOR(Grid::update), 1.0f);
}

#pragma mark - Public methods

void Grid::rotateActiveTetromino()
{
    if (this->activeTetromino)
    {
        this->activeTetromino->rotate(true);
    }

    // TODO: check if collision, undo rotation
}
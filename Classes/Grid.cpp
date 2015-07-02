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
}

#pragma mark - Public methods

void Grid::spawnTetromino(Tetromino *tetromino)
{
    this->activeTetromino = tetromino;
    this->addChild(this->activeTetromino);

    // TODO: Plase a tetromino in correct position in grid
}

void Grid::rotateActiveTetromino()
{
    if (this->activeTetromino)
    {
        this->activeTetromino->rotate(true);
    }

    // TODO: check if collision, undo rotation
}

#pragma mark - Private methods
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

bool Grid::init() {
    if (!Sprite::initWithFile("grid.png")) {
        return false;
    }

    return true;
}
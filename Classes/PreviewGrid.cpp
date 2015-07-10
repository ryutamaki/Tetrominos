//
//  PreviewGrid.cpp
//  Tetrominos
//
//  Created by ryutamaki on 2015/07/10.
//
//

#include "PreviewGrid.h"

USING_NS_CC;

bool PreviewGrid::init()
{
    if (!Sprite::initWithFile("grid.png"))
    {
        return false;
    }

    return true;
}

#pragma mark - View lifecycle

void PreviewGrid::onEnter()
{
    Node::onEnter();
}

void PreviewGrid::onExit()
{
    Node::onExit();
}

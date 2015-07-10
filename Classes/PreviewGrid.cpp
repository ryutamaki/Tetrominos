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

#pragma mark - Public methods

void PreviewGrid::setState(JSONPacker::GameState state)
{
    this->removeAllChildren();

    std::vector<std::vector<Color3B>> board = state.board;

    for (int column = 0; column < board.size(); column++)
    {
        for (int index = 0; index < board[column].size(); index++)
        {
            Color3B color = board[column][index];

            if (color.equals(Color3B::WHITE))
            {
                continue;
            }

            Sprite* block = Sprite::create("block.png");
            Size blockSize = block->getContentSize();
            block->setColor(color);
            block->setPosition(Vec2(index * blockSize.width, column * blockSize.height));

            this->addChild(block);
        }
    }
}

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

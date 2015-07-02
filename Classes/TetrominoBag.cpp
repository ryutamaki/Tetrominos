//
//  TetrominoBag.cpp
//  Tetrominos
//
//  Created by ryutamaki on 2015/07/02.
//
//

#include "TetrominoBag.h"

TetrominoBag::TetrominoBag() :
bag(std::vector<TetrominoType>()),
randomGenerator(std::default_random_engine((unsigned int)time(0)))
{

}

TetrominoType TetrominoBag::getTetrominoType()
{
    if (this->bag.empty())
    {
        std::vector<TetrominoType> allTypes = {
            TetrominoType::I,
            TetrominoType::O,
            TetrominoType::T,
            TetrominoType::S,
            TetrominoType::Z,
            TetrominoType::J,
            TetrominoType::L
        };
        std::shuffle(allTypes.begin(), allTypes.end(), randomGenerator);

        for (TetrominoType tetrominoType : allTypes)
        {
            this->bag.push_back(tetrominoType);
        }
    }

    TetrominoType retValue = this->bag.back();
    this->bag.pop_back();
    return retValue;
}
//
//  TetrominoBag.h
//  Tetrominos
//
//  Created by ryutamaki on 2015/07/02.
//
//

#ifndef __Tetrominos__TetrominoBag__
#define __Tetrominos__TetrominoBag__

#include "Constants.h"

class TetrominoBag
{
public:
    TetrominoBag();

    TetrominoType getTetrominoType();
private:
    std::vector<TetrominoType> bag;
    std::default_random_engine randomGenerator;
};

#endif /* defined(__Tetrominos__TetrominoBag__) */

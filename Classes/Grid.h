//
//  Grid.h
//  Tetrominos
//
//  Created by ryutamaki on 2015/07/01.
//
//

#ifndef __Tetrominos__Grid__
#define __Tetrominos__Grid__

class Tetromino;

class Grid : public cocos2d::Sprite {
public:
    CREATE_FUNC(Grid);

    void spawnTetromino(Tetromino* tetromino);
    void rotateActiveTetromino();

private:
    Tetromino* activeTetromino;

    bool init() override;
    void onEnter() override;
};

#endif /* defined(__Tetrominos__Grid__) */

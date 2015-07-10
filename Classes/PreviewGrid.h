//
//  PreviewGrid.h
//  Tetrominos
//
//  Created by ryutamaki on 2015/07/10.
//
//

#ifndef __Tetrominos__PreviewGrid__
#define __Tetrominos__PreviewGrid__

#include "cocos2d.h"
#include "JSONPacker.h"

class PreviewGrid : public cocos2d::Sprite
{
public:
    CREATE_FUNC(PreviewGrid);

private:
    bool init() override;
    void onEnter() override;
    void onExit() override;

};

#endif /* defined(__Tetrominos__PreviewGrid__) */

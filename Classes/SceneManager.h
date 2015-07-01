//
//  SceneManager.h
//  Tetrominos
//
//  Created by ryutamaki on 2015/07/01.
//
//

#ifndef __Tetrominos___SceneManager__
#define __Tetrominos___SceneManager__

#include "cocos2d.h"

class SceneManager {
public:
    static SceneManager* getInstance();

    void enterGameScene(bool networked);

protected:
    SceneManager();
    ~SceneManager();

};

#endif /* defined(__Tetrominos___SceneManager__) */

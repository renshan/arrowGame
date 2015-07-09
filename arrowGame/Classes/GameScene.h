//
//  GameScene.h
//  arrowGame
//
//  Created by renshan on 15/6/18.
//
//

#ifndef __arrowGame__GameScene__
#define __arrowGame__GameScene__

#include <iostream>
#include "cocos2d.h"
#include "Player.h"

USING_NS_CC;

class GameScene : public Layer
{
public:
    GameScene();

    static cocos2d::Scene* createScene();
    virtual bool init();
    
    void addGameBg();
    Animation* createAnimation(std::string prefixName, int framesNum, float delay);

    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);

    void menuCloseCallback(Ref* pSender);
    CREATE_FUNC(GameScene);
    
private:
    Size winSize;

    Player* player;
    Point playerPos;
    
    Point preTouchPoint;
    Point currTouchPoint;
    
    cocos2d::Layer* gameLayer;
    cocos2d::Layer* toolsLayer;
};

#endif /* defined(__arrowGame__GameScene__) */

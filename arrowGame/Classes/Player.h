//
//  Player.h
//  arrowGame
//
//  Created by renshan on 15/6/23.
//
//

#ifndef __arrowGame__Player__
#define __arrowGame__Player__

#include "cocos2d.h"
#include "ArrowPathBezier.h"
USING_NS_CC;

class Player: public Sprite
{
public:
    Player();
        
    bool init(Vec2 playerPos);
    static Player* create(Vec2 playerPos);
    
    void createPlayer();
    void createPlayerHpBar();
    void shootArrow();
    void rotateArrow(Point touchPoint);
    void createAndShootArrow( Point touchPoint);
    void update(float dt);
    void finishRunAction();
    
    CC_SYNTHESIZE(int, playerHp, PlayerHp);
    CC_SYNTHESIZE(bool, startDraw, StartDraw);
    CC_SYNTHESIZE(bool, isRunAction, IsRunAction);
    
private:
    Vec2 playerPos;
    Size winSize;
    Size playerSize;
    Sprite* playerbody;
    Sprite* playerarrow;
    ProgressTimer* hpBar;
    ccQuadBezierConfig bezier;
    DrawNode* drawNode;
    Sprite* hPBgSprite;
};
#endif /* defined(__arrowGame__Player__) */

//
//  Player.cpp
//  arrowGame
//
//  Created by renshan on 15/6/23.
//
//

#include "Player.h"

Player::Player()
{
    winSize = Director::getInstance()->getWinSize();
    playerarrow = NULL;
    hpBar = NULL;
    drawNode = NULL;
    bezier.controlPoint = Vec2(0, 0);
    bezier.endPosition = Vec2(0, 0);
}

Player * Player::create(Vec2 playerPos)
{
    Player *pRet  = new Player();
    
    if (pRet && pRet->init(playerPos))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool Player::init(Vec2 playerPos)
{
    if (!Sprite::init())
    {
        return false;
    }
    
    this->playerPos = playerPos;
    
    createPlayer();
    createPlayerHpBar();
    
    scheduleUpdate();
    return true;
}

void Player::createPlayer()
{
    playerbody = Sprite::createWithSpriteFrameName("playerbody.png");
    playerSize = Size(playerbody->getContentSize().width/2, playerbody->getContentSize().height / 3*2);
    playerbody->setAnchorPoint(Vec2(0.7f, 0.4f));
    this->addChild(playerbody);
    this->setPosition(Vec2(playerPos.x /*+ GameManager::getInstance()->objectPosOffX*/, playerPos.y + playerSize.height * 0.4f));
    
    
    playerarrow = Sprite::createWithSpriteFrameName("playerarrow.png");
    playerarrow->setPosition(Vec2(0, 0));
    playerarrow->setAnchorPoint(Vec2(0.2f, 0.5f));
    this->addChild(playerarrow);
    
}

void Player::createPlayerHpBar()
{
    hPBgSprite = Sprite::createWithSpriteFrameName("hpbg.png");
    hPBgSprite->setPosition(Vec2(playerbody->getContentSize().width / 2, playerbody->getContentSize().height));
    playerbody->addChild(hPBgSprite);
    hpBar = ProgressTimer::create(Sprite::createWithSpriteFrameName("hp1.png"));
    hpBar->setType(ProgressTimer::Type::BAR);
    hpBar->setMidpoint(Vec2(0, 0.5f));
    hpBar->setBarChangeRate(Vec2(1, 0));
    hpBar->setPercentage(100);
    hpBar->setPosition(Vec2(hPBgSprite->getContentSize().width / 2, hPBgSprite->getContentSize().height / 2 ));
    hPBgSprite->addChild(hpBar);
    hPBgSprite->setVisible(false);
}

void Player::rotateArrow(Point touchPoint)
{
    auto playerPos = this->getPosition();
    auto pos = playerPos + playerarrow->getPosition();
    
    Point vector = touchPoint - pos;
    auto rotateRadians = vector.getAngle();
    auto rotateDegrees = CC_RADIANS_TO_DEGREES( -1 * rotateRadians);
    
    if (rotateDegrees >= -180 && rotateDegrees <= -90){
        rotateDegrees = -90;
    }
    else if (rotateDegrees >= 90 && rotateDegrees <= 180){
        rotateDegrees = 90;
    }
    auto speed = 0.5 / M_PI;
    auto rotateDuration = fabs(rotateRadians * speed);
    playerarrow->runAction( Sequence::create(RotateTo::create(rotateDuration, rotateDegrees),
                                             NULL));
    
    // 检测路径贝赛尔的控制点和终点
    auto MAX_DISTANCE = winSize.width / 2;
    auto x =MAX(MAX_DISTANCE/10, MIN(vector.x, MAX_DISTANCE));
    auto y = vector.y;
    
    Point endPoint = Vec2(4 * x, -this->convertToWorldSpace(Vec2(4 * x, y > 0 ? 0 : std::abs(y*10))).y);
    
    Point q = Point(2*x, y*2);
    bezier.controlPoint = q;
    bezier.endPosition = endPoint;
    startDraw = true;
}

void Player::createAndShootArrow( Point touchPoint)
{
    isRunAction = true;
    
    auto animation = AnimationCache::getInstance()->getAnimation("player");
    auto animate = Animate::create(animation);
    auto delayTime = DelayTime::create(0.5f);
    auto funcall1= CallFunc::create(CC_CALLBACK_0(Player::shootArrow, this));
    auto funcall2= CallFunc::create(CC_CALLBACK_0(Player::finishRunAction, this));
    playerbody->runAction(Sequence::create(animate, funcall1, delayTime, funcall2, NULL));
}

void Player::shootArrow()
{
    Sprite* arrow = Sprite::createWithSpriteFrameName("arrow1.png");
    arrow->setRotation(playerarrow->getRotation());
    arrow->setPosition(Vec2(playerarrow->getPosition().x/2, playerarrow->getPosition().y));
    this->addChild(arrow);
    
    auto action = ArrowPathBezier::create(2, bezier);
    arrow->runAction(action);
}

void Player::finishRunAction()
{
    isRunAction = false;
}

void Player::update(float dt)
{
    if(!startDraw){
        return;
    }
    if(drawNode){
        drawNode->removeFromParentAndCleanup(true);
    }
    drawNode = DrawNode::create();
    drawNode->drawQuadBezier(Vec2(playerarrow->getPosition().x/2, playerarrow->getPosition().y), bezier.controlPoint,  bezier.endPosition, 100, Color4F(1.0, 0.0, 0.0, 1.0));

    this->addChild(drawNode, -2);
    
}
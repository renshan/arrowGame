//
//  GameScene.cpp
//  arrowGame
//
//  Created by renshan on 15/6/18.
//
//

#include "GameScene.h"

#define MAP_WIDTH 27
#define MAP_HEIGHT 15

GameScene::GameScene()
{
    winSize = Director::getInstance()->getWinSize();
    player = NULL;
    preTouchPoint = Vec2(0, 0);
    currTouchPoint = Vec2(0, 0);
}
Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();

    scene->addChild(layer);

    return scene;
}


bool GameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("texture.plist", "texture.pvr.ccz");
    
    auto animation = createAnimation("player", 8, 0.06f);
    AnimationCache::getInstance()->addAnimation(animation, "player");
    
    addGameBg();
    
    player = Player::create(Vec2(winSize.width / 4, winSize.height/5));
    this->addChild(player);
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan,this);
    listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved,this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded,this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}


Animation* GameScene::createAnimation(std::string prefixName, int framesNum, float delay)
{
    Vector<SpriteFrame*> animFrames;
    
    for (int i = 1; i <= framesNum; i++)
    {
        char buffer[20] = { 0 };
        sprintf(buffer, "%i.png",  i);
        std::string str =  prefixName + buffer;
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        animFrames.pushBack(frame);
    }
    return Animation::createWithSpriteFrames(animFrames, delay);
}

void GameScene::addGameBg(){
    
    Sprite* spGameBg = Sprite::create("bg1.jpg");
    spGameBg->setPosition(Vec2(winSize.width/2, winSize.height/2));
    this->addChild(spGameBg, -1);
    
    auto map = TMXTiledMap::create("map.tmx");
    map->setAnchorPoint(Vec2(0.5f, 0));
    map->setPosition(Vec2(winSize.width / 2, 0));
    this->addChild(map, -1);
//
//    objectGroup = map->getObjectGroup("object");
//    objectPosOffX = -(map->getContentSize().width - winSize.width) / 2;
}

bool GameScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    currTouchPoint = touch->getLocation();
    if( !currTouchPoint.equals(preTouchPoint)){
        player->rotateArrow(currTouchPoint);
        preTouchPoint = currTouchPoint;
    }
    return true;
}

void GameScene::onTouchMoved(Touch *touch, Event *unused_event)
{
    currTouchPoint = touch->getLocation();
    if( !currTouchPoint.equals(preTouchPoint)){
        player->rotateArrow(currTouchPoint);
        preTouchPoint = currTouchPoint;
    }
}

void GameScene::onTouchEnded(Touch *touch, Event *unused_event)
{
    player->createAndShootArrow(currTouchPoint);
}


void GameScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

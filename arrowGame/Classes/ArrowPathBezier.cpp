//
//  ArrowPathBezier.cpp
//  arrowGame
//
//  Created by renshan on 15/6/23.
//
//

#include "ArrowPathBezier.h"

// 原来贝赛尔曲线的计算公式
// Bezier cubic formula:
//    ((1 - t) + t)3 = 1
// Expands to
//   (1 - t)3 + 3t(1-t)2 + 3t2(1 - t) + t3 = 1
static inline float bezierat( float a, float b, float c, float d, float t )
{
    return (powf(1-t,3) * a +
            3.0f*t*(powf(1-t,2))*b +
            3.0f*powf(t,2)*(1-t)*c +
            powf(t,3)*d );
}

// 二次贝赛尔计算公式
static inline float bezierat1( float a, float b, float d, float t )
{
    return (powf(1-t,2) * a +
            2.0f*t*(1-t)*b +
            powf(t,2)*d );
}

ArrowPathBezier::ArrowPathBezier()
{
}

ArrowPathBezier* ArrowPathBezier::create(float t, const ccQuadBezierConfig& c)
{
    ArrowPathBezier *pBezierBy = new (std::nothrow) ArrowPathBezier();
    pBezierBy->initWithDuration(t, c);
    pBezierBy->autorelease();
    
    return pBezierBy;
}

bool ArrowPathBezier::initWithDuration(float t, const ccQuadBezierConfig& c)
{
    bool bRet = false;
    
    if (CCActionInterval::initWithDuration(t))
    {
        _config = c;
        return true;
    }
    
    return bRet;
}

void ArrowPathBezier::update(float time)
{
    if (_target) {
        float xa = 0;
        float xb = _config.controlPoint.x;
        float xd = _config.endPosition.x;
        
        float ya = 0;
        float yb = _config.controlPoint.y;
        float yd = _config.endPosition.y;
        
        float x = bezierat1(xa, xb, xd, time);
        float y = bezierat1(ya, yb, yd, time);
        
#if CC_ENABLE_STACKABLE_ACTIONS
        Vec2 currentPos = _target->getPosition();
        Vec2 diff = currentPos - _previousPosition;
        _startPosition = _startPosition + diff;
        
        Vec2 newPos = _startPosition + Vec2(x,y);
        _target->setPosition(newPos);
        
        _previousPosition = newPos;
        
        if (0 != time) {
            Vec2 vector = currentPos - newPos;
            float radians = vector.getAngle();
            float degrees = CC_RADIANS_TO_DEGREES(-1 * radians);
            _target->setRotation(degrees - 180);
        }
        
#else
        _target->setPosition( _startPosition + Vec2(x,y));
#endif // !CC_ENABLE_STACKABLE_ACTIONS
        
    }
}
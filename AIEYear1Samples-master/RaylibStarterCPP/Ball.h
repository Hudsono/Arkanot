#pragma once
#include "CircleObject.h"
#include "Helper.h"
#include <algorithm>
#include "Paddle.h"
#include "Brick.h"

class Ball : public CircleObject
{
    using CircleObject::CircleObject;
public:
    Ball(Vector2 spawn, Vector2 direction);
    ~Ball();

    void Render();
    void Update(float deltaTime);
    void MoveBall(float deltaTime); //Ball movement and basic boundary-screen collision


    static std::vector<Ball*> _ballList;
    static int _ballIDTotal;
    int _ballID;

    Vector2 _prevPos;
    float _velocity;

    static float _ballSpeedMod;    // Speed modification to all balls. Used in the Slow powerup.

    Paddle* _stuckPaddle;   //paddle to be stuck to. If nullptr, this ball is not stuck to a pointer.
    void StickToPaddle(Paddle* paddle); //Stick this ball to a given paddle
    float _stuckOffset;     //the X position offset this ball was stuck to the paddle at

    void Disrupt();     //Spawns 2 more balls on top of this ball--Disruption powerup

    void BallRectColRes(RectObject* rectobj);    //Generic ball collision for rectangular objects -- enemies and bricks
};


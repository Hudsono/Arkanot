#pragma once
#include "RectObject.h"

class Ball; //forward declaration so we can reference the Ball class, because Ball.h already includes Paddle.h

class Paddle : public RectObject
{
    using RectObject::RectObject;

public:
    Paddle(Vector2 spawn);
    void Render();
    void Update(float deltaTime);

    //States the paddle /can be/ in
    enum class PaddleState
    {
        Normal,     //Normal operation
        Dead,       //Lost all lives--inoperable. Not necessarily removed from the game.
        Laser,      //Laser power-up
        Enlarge,    //Larger paddle power-up
        Catch       //Balls stick to paddle power-up
    };

    //State the paddle /is/ in
    PaddleState _paddleState;

    //List of balls which have been stuck to this paddle
    vector<Ball*> _stuckBalls;

    //Controls for the paddle
    void MoveLeft();    //A key
    void MoveRight();   //D key
    void Fire();        //M1 or space. Multi-purpose--fires balls off the paddle, fires lasers, etc.

    //Timer to limit how long balls may be stuck to the paddle for.
    //Sticking resets the timer to _stuckTimerMax.
    //Once timer reaches 0, Fire() is called automatically.
    int _stuckTimer;    
    static int _stuckTimerMax;

    //Collision resolver with balls
    void PaddleBallColRes(Ball* ball);

    //Score count for this particular paddle.
    int _score;

    //Resets the paddle's state, position and spawns a ball stuck to the paddle.
    void ResetPaddle();
};


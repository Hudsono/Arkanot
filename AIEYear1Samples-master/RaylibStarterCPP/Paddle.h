#pragma once
#include "GameObject.h"

class Ball; //forward declaration so we can reference the Ball class, because Ball.h already includes Paddle.h

class Paddle : public GameObject
{
    using GameObject::GameObject;

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
};


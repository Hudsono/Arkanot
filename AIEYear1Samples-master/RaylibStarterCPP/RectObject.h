#pragma once
#include "GameObject.h"
#include "CircleObject.h"

//Paddles, bricks, enemies, powerups

class RectObject : public GameObject
{
public:
    using GameObject::GameObject;

    RectObject(Vector2 spawn, const char* textureFile = "none.png");
    ~RectObject();

    void RenderDebug(); //Add rectangle draw

    //The possible results of a collision with a RectObject to be returned to another object--say, Balls/CircleObjects.
    enum class RectColResult
    {
        None,       //No collision made.
        Top,        //Collision made, touched the top side.
        Bottom,     //Collision made, touched the bottom side.
        Left,       //Collision made, touched the left side.
        Right       //Collision made, touched the right side.
    };

    //Evaluate the collision with a CircleObject
    RectColResult RectCircleCollision(CircleObject* circle);
};


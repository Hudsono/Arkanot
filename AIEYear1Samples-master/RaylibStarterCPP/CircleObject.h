#pragma once
#include "GameObject.h"

//Balls, bullets

class CircleObject : public GameObject
{
protected:
    using GameObject::GameObject;

    CircleObject(Vector2 spawn, const char* textureFile = "none.png");
    ~CircleObject();

    void RenderDebug(); //Add circle draw

public:
	float _radius;  //Radius of the circle object.
};


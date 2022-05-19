#pragma once
#include "GameObject.h"

//Paddles, bricks, enemies, powerups

class RectObject : public GameObject
{
public:
    using GameObject::GameObject;

    RectObject(Vector2 spawn, const char* textureFile = "none.png");
    ~RectObject();

    void RenderDebug(); //Add rectangle draw
};


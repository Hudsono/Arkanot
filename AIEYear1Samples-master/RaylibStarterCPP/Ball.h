#pragma once
#include "GameObject.h"
class Ball : public GameObject
{
    using GameObject::GameObject;
    Ball();

public:
    void Render();
    void Update(float deltaTime);
};


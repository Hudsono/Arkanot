#pragma once
#include "GameObject.h"
class Paddle : public GameObject
{
    using GameObject::GameObject;
    Paddle();

public:
    void Render();
    void Update(float deltaTime);
};


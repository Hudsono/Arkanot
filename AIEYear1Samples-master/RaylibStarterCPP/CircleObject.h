#pragma once
#include "GameObject.h"
class CircleObject : public GameObject
{
public:
    using GameObject::GameObject;

    CircleObject(Vector2 spawn);
    ~CircleObject();

	float _size;
};


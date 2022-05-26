#pragma once
#include "RectObject.h"
#include "Helper.h"

class Brick; // Forward declaration so we can reference the Brick class.

class Bullet : public RectObject
{
    using RectObject::RectObject;
public:
    Bullet(Vector2 spawn);
    ~Bullet();

    void Render();
    void Update(float deltaTime);

    void BulletBrickCol(Brick* brick);  // Check if this bullet collided with a given brick; action on both objects if so.


    static std::vector<Bullet*> _bulletList;
    static int _bulletIDTotal;
    int _bulletID;
};


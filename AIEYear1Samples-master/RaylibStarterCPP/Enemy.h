#pragma once
#include "RectObject.h"
#include "Helper.h"

class Enemy : public RectObject
{
    using RectObject::RectObject;
public:
    Enemy(Vector2 spawn);
    ~Enemy();

    void Render();
    void Update(float deltaTime);


    static std::vector<Enemy*> _enemyList;
    static int _enemyIDTotal;
    int _enemyID;

    // Types this Enemy can be. Purely aesthetic.
    enum class EnemyTypes
    {
        Cone,
        Spheres,
        Pyramid,
        Cube,
    };

    // The current type this enemy is.
    EnemyTypes _enemyType;
};


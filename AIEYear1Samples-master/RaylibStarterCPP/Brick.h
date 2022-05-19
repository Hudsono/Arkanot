#pragma once
#include "RectObject.h"
class Brick : public RectObject
{
    using RectObject::RectObject;

public:
    Brick(Vector2 spawn, char inputType);
    ~Brick();
    void Render();
    void Update(float deltaTime);

    //Colours the brick can be and their respective health 
    enum class BrickTypes
    {
        Gold = 1,   //Invincible; cannot be broken.
        Silver,     //3 hits to break.
        Red,        //This and following colours break in 1 hit.
        Green,
        Yellow,
        Orange,
        White,
        Cyan,
        Blue,
        Magenta
    };

    int _brickHealth;   //How many times the ball must hit the brick for it to be destroyed. -1 means invincible.

    //The colour type of this brick
    BrickTypes _brickType;

    static std::vector<Brick*> _brickList;
    static int _brickIDTotal;
    int _brickID;

protected:
    static Color _colMap[];
    Color SetBrickType(char inType);
};


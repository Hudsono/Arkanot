#pragma once
#include "RectObject.h"
#include "Ball.h"

class Brick : public RectObject
{
    using RectObject::RectObject;

public:
    //Constructor.
    //Needs to know its 2D position in the level array, which will determine its position.
    //inputType = colour of the brick to initialise, determining other properties of this brick.
    Brick(Vector2 levelPos, char inputType);

    //Destructor
    ~Brick();

    //Renders the brick graphic
    void Render();


    void Update(float deltaTime);

    //Colours the brick can be and their respective health 
    enum class BrickTypes
    {
        Gold = 1,   // Invincible; cannot be broken.
        Silver,     // 2 hits to break initially--health increases by 1 every 8 stages. Points = 50 * stage number (regardless of multiples of 8)
        Red,        // This and following colours break in 1 hit.
        Green,
        Yellow,
        Orange,
        White,
        Cyan,
        Blue,
        Magenta,
        Black       // There are no black bricks in the game, so this tells us that something went wrong in colour recognition/level loading.
    };

    int _brickHealth;   //How many times the ball must hit the brick for it to be destroyed. -1 means invincible.

    //The colour type of this brick
    BrickTypes _brickType;

    //List of pointers to all Brick objects
    static std::vector<Brick*> _brickList;

    //
    static int _brickIDTotal;
    int _brickID;

    //Record what position in the entire level this brick is placed.
    Vector2 _levelPos;

    //Influence the collided ball's physics and check brick health --NOT USED
    //void BrickBallColRes(Ball* ball);

    //React to a bullet or ball impacting this brick.
    //Impact effects, decrease health and/or break the brick.
    void ImpactBrick();

    //Breaks the brick when its health is 0
    void Break();


    void Shine();       // Quickly flashes the sprite white to show the player this brick is harder to destroy (Silver and Gold bricks)
    int _shineTime;     // How long to shine for...

protected:
    static Color _colMap[];
    
    //Sets the brick's type given a letter. 
    //Most lowercase letters correspond to the first letter of its respective colour.
    //Gold and Green share the same first letter; thus, 'g' = Green and 'G' = Gold.
    Color SetBrickType(char inType);
};


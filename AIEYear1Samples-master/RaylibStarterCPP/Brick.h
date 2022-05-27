#pragma once
#include "RectObject.h"
#include "Ball.h"

class Brick : public RectObject
{
    using RectObject::RectObject;

public:
    // Constructor.
    // Needs to know its 2D position in the level array, which will determine its position.
    // inputType = colour of the brick to initialise, determining other properties of this brick.
    Brick(Vector2 levelPos, char inputType);

    // Destructor
    ~Brick();

    // Renders the brick graphic
    void Render();


    void Update(float deltaTime);

    // Colours the brick can be and their respective point rewards when destroyed 
    enum class BrickTypes
    {
        Gold = 0,        // Invincible; cannot be broken.
        Silver = 50,     // 2 hits to break initially--health increases by 1 every 8 stages. Points = 50 * stage number (regardless of multiples of 8)
        White = 50,      // This and following colours break in 1 hit.
        Orange = 60,
        Cyan = 70,
        Green = 80,
        Red = 90,
        Blue = 100,
        Magenta = 110,
        Yellow = 120,
        Black = 0       // There are no black bricks in the game, so this tells us that something went wrong in colour recognition/level loading.
    };

    int _brickHealth;   //How many times the ball must hit the brick for it to be destroyed. -1 means invincible.

    BrickTypes _brickType;    // The colour type of this brick

    static std::vector<Brick*> _brickList;    // List of pointers to all Brick objects

    static int _brickIDTotal;
    int _brickID;

    static int _numBricksToDestroy; // Counts how many bricks there are to destroy to win the level. Does not count Gold bricks as they cannot break.

    Vector2 _levelPos;    // Record what position in the entire level this brick is placed.

    // React to a bullet or ball impacting this brick.
    // Impact effects, decrease health and/or break the brick.
    void ImpactBrick();

    void Break();       // Breaks the brick when its health is 0

    // Checks the number of non-gold bricks present.
    // Returns True if that number is 0; false if otherwise.
    static bool CheckBrickWin();


    void Shine();       // Quickly flashes the sprite white to show the player this brick is harder to destroy (Silver and Gold bricks)
    int _shineTime;     // How long to shine for...

protected:
    // Sets the brick's type given a letter. 
    // Most lowercase letters correspond to the first letter of its respective colour.
    // Gold and Green share the same first letter; thus, 'g' = Green and 'G' = Gold.
    Color SetBrickType(char inType);
};


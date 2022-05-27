#pragma once
#include "RectObject.h"
#include "Helper.h"

class Powerup : public RectObject
{
    using RectObject::RectObject;
public:
    Powerup(Vector2 spawn);
    ~Powerup();

    void Render();
    void Update(float deltaTime);


    static std::vector<Powerup*> _powerupList;
    static int _powerupIDTotal;
    int _powerupID;

    // Types this Powerup can be -- affects colour and effect given to the player
    enum class PowerupTypes
    {
        Laser = 0,      // Gives paddle ability to fire bullets
        Enlarge,    // Makes the paddle wider
        Catch,      // Allows balls to stick to the paddle
        Slow,       // Slows down all balls for a short while
        Break,      // Allows the paddle to advance to the next level
        Disruption, // Spawns more balls on top of all existing balls
        Life        // Grants the player 1 more life
    };

    // The current type this power-up is
    PowerupTypes _powerupType;

    void EnlargePaddle();   // Sets the paddle state to Enlarge and plays an animation
};


#pragma once
#include "raylib.h"

class Player
{
	//Here, point to a paddle object for the player to control
	//Also, define the controls for the player here.
	//Also define the score for the player here.

public:
	//static 

	//Store the player's individual score
	int _score;

	//Store the player's controls
	KeyboardKey _keyLeft;
	KeyboardKey _keyRight;
	KeyboardKey _keyFire;
};


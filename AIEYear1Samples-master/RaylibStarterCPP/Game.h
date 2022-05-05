#pragma once


/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS


//#include "raygui.h"

#include <sstream>
#include <string.h>

#include "GameObject.h"
#include "Paddle.h"
#include "Ball.h"
#include "Stopwatch.h"

#define MAX_BALLS 10

class Game
{
public:
	Game();

	//basic recipe for a real-time game

	//Initialises the game object.
	void Init();

	//Updates the game object's logic.
	void Update();

	//Draws the game's graphical elements.
	void Draw();

	//De-initialises the game object.
	//Returns: an int as a desired stop code, if necessary; 0 by default.
	int DeInit();



	Paddle* _paddle;
	Ball* _ball;

	Ball* _ballList[MAX_BALLS];
	static int _numBalls;	//TODO: Last here. Want to keep track of the number of balls in the scene so we can loop through and update
							//them in Game.cpp. Want to use this formula for multiple bricks, bullets, etc. as well...

	~Game();

protected:

	long currentTime;
	long lastTime;
	float timer;
	int fps;
	int frames;

	Stopwatch stopWatch;

	float deltaTime;


	int screenWidth;
	int screenHeight;

	int screenMidX;
	int screenMidY;
};


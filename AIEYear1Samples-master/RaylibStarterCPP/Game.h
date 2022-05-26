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

#define SCREEN_WIDTH	1280
#define SCREEN_HEIGHT	720


//#include "raygui.h"

#include <sstream>
#include <string.h>
#include <stdlib.h>	// Math absolute (abs)

#include "GameObject.h"
#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"
#include "Level.h"
#include "Powerup.h"
#include "Enemy.h"
#include "Stopwatch.h"



class Game
{
public:
	// Constructor
	Game();
	// Destructor
	~Game();

	// Basic recipe for a real-time game

	// Initialises the game object.
	void Init();

	// Updates the game object's logic.
	void Update();

	// Draws the game's graphical elements.
	void Draw();

	// De-initialises the game object.
	// Returns: an int as a desired stop code, if necessary; 0 by default.
	int DeInit();



	Paddle* _paddle;				// Points to the main player paddle. Might be more later on for multiplayer... (i.e. convert to vector)
	static Rectangle _boundary;		// Boundary of the play area that entities are confined to.
	Vector2 _borderPadding;			// The amount of X, Y padding from the game window and the boundary
	//float _boundRight;
	//float _boundBottom;


	static bool _debugMode;	// Showing debug graphics; boundaries; vectors; on screen displays; etc.

	static bool _paused;	// Stop logic updates for the game.

	void DoPhysics();			// Compute physics checks between entities using xyzPhysicsCheck() functions
	void BallPhysicsCheck();	// Check each ball against each brick, each other ball, each enemy and each paddle.
	void BulletPhysicsCheck();	// Check each bullet against each brick, each ball and each enemy
	void EnemyPhysicsCheck();	// Check each enemy against each brick
	void PaddlePhysicsCheck();	// Check each paddle against each power-up

	//int PhysCircleRect(GameObject* rect, GameObject* circ);	// Return > 0 if the rectangle and circle interset, and where on the rectangle via 1-4; 0 = no collision.
	
	static Level _level;	// The current level of the game.
	static int _levelNum;	// Keeps track of what level the player is currently on.
	void SetLevel(int level);	// Destroys current level, then checks if we can load a next available level.
	
	void LoadLevel(std::string levelPath);	// Load graphics of a level, its bricks, etc.

	//char _level[20][13];	// Y/X or columns/rows of the current level

	void WinGame();		// Win the current game; advance to the next level if available.
	void LoseLife();	// Paddle is destroyed and the player loses a life; if there are no more lives, GameOver(), otherwise the paddle respawns back in the current level.
	void GameOver();	// Game returns to the main menu and all progress is lost.
	
	void ClearEntities();	// Deletes all non-brick entities. Used when a life is lost; the destroyed bricks remain in the level.

	static int _lives;		// Number of lives shared between all possible paddles. Decrements upon no balls being present. GameOver() upon 0 lives.
	static int _score;		// Score shared between all possible paddles.

	void SlowPowerup();	// Sets the Ball class' timer for its ball speed modifier. Used by the Slow powerup.
	int _slowTimer;		// Timer to count down for ball speed modifier. Reaching 0 sets the ball speed back to 1x.


protected:
	// Timer variables.
	long currentTime;
	long lastTime;
	float timer;
	int fps;
	int frames;

	Stopwatch stopWatch;

	float deltaTime;


	int screenWidth;
	int screenHeight;

	// Points representing the middle of the screen
	int screenMidX;
	int screenMidY;
};


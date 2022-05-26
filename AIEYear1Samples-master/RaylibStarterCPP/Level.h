#pragma once
#include "Brick.h"
#include <fstream>

//#define COLUMNS 20
#define COLUMNS 18
#define ROWS 13

class Level
{
public:
	//Constructor.
	//levelPath is the file path to the .txt level file to load in as a Level object--this object.
	//Initialises Level properties; does not load interactive objects. Fills _levelBrickTypes.
	Level(std::string levelPath);

	//Destructor
	~Level();

	// Loads interactive objects. Fills _levelBrickPtrs.
	void InitLevel();

	// Destroys all bricks to make way for the next level.
	void DeInitLevel();

	//2D array of Brick types to be loaded in as Brick Pointers in _levelBrickPtrs
	//Used during Level's initialisation.
	char _levelBrickTypes[COLUMNS][ROWS];

	//2D array of Brick pointers
	//Used during Level's gameplay.
	Brick* _levelBrickPtrs[COLUMNS][ROWS];	//y,x or columns/rows of the current level

	//Removes a brick from _levelBrickPtrs--sets it to nullptr
	void RemoveBrick(int col, int row);

	void LoadLevelFile(std::string filePath);
};


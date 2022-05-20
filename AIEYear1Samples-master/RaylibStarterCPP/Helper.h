#pragma once

#include "raylib.h"
#include <math.h>
#include <vector>
#include <algorithm>
#include <map>
#include <iostream>

//Directory of all resource files to prepend given file names
static std::string FILE_PATH = "../resources/";

//Stores the RayLib Image data and its associated RayLib Texture.
struct Sprite
{
	Image image;
	Texture texture;
};

class Helper
{
public:
	//Get the magnitude of a 2D vector (raylib)
	static float Magnitude(Vector2 input);
	//Normalise a 2D vector (raylib)
	static void Normalise(Vector2 &input);	
	//See the normalisation of a 2D vector (raylib)
	static Vector2 Normalised(Vector2 input);	
	//Dot product of two 2D vectors (raylib)
	static float Dot(Vector2& input1, Vector2& input2);	

	static void SetRect(Rectangle& input, float xPos, float yPos, float width, float height);
	//For float ranges between -1 and 1, gets the closest real number.
	//Returns -1 if input < 0; returns 1 if input > 0; returns 0 if input == 0.
	static int FloorCeil(float input);

	//Clamps a given value between the given minimum value and given maximum value.
	//Returns min if input < min; returns max if input > max; returns input if min < input < max.
	static float Clamp(float input, float min, float max);

	//Clamps a given value outside of the given minimum value and given maximum value (inclusive).
	//Returns min if min < input < (max+min)/2; returns max if (max+min)/2 < input < max; returns either max or min at random if input == (max+min)/2.
	static float ClampOut(float input, float min, float max);

	//Checks if either value of the input is NaN
	//Returns true if either value is NaN; false if both are not NaN.
	static bool isNaNVector(Vector2 input);

	//Retains a list of all RayLib image file paths, so that an image will not be loaded twice. Used by LoadImageOp();
	static std::map<const char*, Image*> _images;

	//Maps a file name to a map of an image and its texture 
	static std::map<std::string, Sprite> _sprites;

	//Loads/sets both the image and texture to the given variables using resource-optimised methods, given the name of the file.
	static void LoadSprite(std::string fileName, Sprite &sprite);

	//Simplifies brick collision resolution based on surrounding bricks in the level
	//AKA if a brick has other bricks above, left, right and below, then don't bother checking any collision
	//if a brick has another brick above, but no others surrounding it, don't bother checking top col
	//etc

	//NOTE: requires me to build or set some kind of Level object. Most probably a struct...
	//Level structs can hold the 2D array of bricks which we'll need here
	//Level structs can also define what kind of enemies spawn, the background, etc...
	//Can stick level structs in an array for the game to progress through
	//Level structs should also have functions to initialise (set up bricks, remove enemies, etc.)
	RESUME FROM HERE!
	static RectObject::RectColResult SimpleBrickCol(Level* level, Ball ball);
};
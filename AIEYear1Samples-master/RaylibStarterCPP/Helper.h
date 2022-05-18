#pragma once

#include "raylib.h"


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
};
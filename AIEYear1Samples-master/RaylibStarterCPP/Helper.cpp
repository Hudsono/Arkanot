#include "Helper.h"
#include <math.h>

float Helper::Magnitude(Vector2 input)
{
    return sqrtf(input.x * input.x + input.y * input.y);
}

void Helper::Normalise(Vector2 &input)
{
    float mag = Magnitude(input);

    input.x /= mag;
    input.y /= mag;
}

Vector2 Helper::Normalised(Vector2 input)
{
    float mag = Magnitude(input);

    input.x /= mag;
    input.y /= mag;

    return input;
}

float Helper::Dot(Vector2& input1, Vector2& input2)
{
    return input1.x * input2.x + input1.y * input2.y;
}

void Helper::SetRect(Rectangle &input, float xPos, float yPos, float width, float height)
{
    input.x = xPos;
    input.y = yPos;
    input.width = width;
    input.height = height;
}

int Helper::FloorCeil(float input)
{
    if (input > 0)
        return ceil(input);
    else if (input < 0)
        return floor(input);
    else
        return input;
}

float Helper::Clamp(float input, float min, float max)
{
    input = input > max ? max : input;  //set input to max if it's > max; otherwise, set to itself
    input = input < min ? min : input;  //set input to min if it's < min; otherwise, set to itself
    return input;   //return the result 
}

float Helper::ClampOut(float input, float min, float max)
{
    //Get the half-way point so we know which side to correct to
    //If the input value is closer to min, we set it to min. If the input value is closer to max, we set it to max.
    //If the input value is exactly halfway, pick a random side.
    float halfWay = min + max / 2;
    
    if (input > halfWay && input < max)             //If halfway < input < max, return max
        input = max;
    else if (input < halfWay && input > min)        //If min < input < halfway, return min
        input = min;
    else if (input == halfWay)                      //if input == halfway, return max or min at random
        input = GetRandomValue(0, 1) ? max : min;

    return input;
}

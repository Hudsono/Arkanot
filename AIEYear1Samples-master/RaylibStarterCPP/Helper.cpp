#include "Helper.h"

//Initialise static variables to keep the linker happy
//std::map<const char*, Image*> Helper::_images;
//std::map<Image*, Texture*> Helper::_textures;

std::map<std::string, Sprite> Helper::_sprites;

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

bool Helper::isNaNVector(Vector2 input)
{
    //If either value of a Vector2 is NaN, return true.
    return (isnan(input.x)) || (isnan(input.y));
}

void Helper::LoadSprite(std::string fileName, Sprite &sprite)
{
    //Prepend the base ../resources/ file path to the given file name
    std::string filePath = fileName.insert(0, FILE_PATH);

    //Create an iterator to find a possible key match with the input file path
    std::map<std::string, Sprite>::iterator itSprites = _sprites.find(filePath);

    //If it found something i.e. didn't return a point after the last vector element, return the key's matching image
    //Otherwise, return a new loaded image of the given file path
    if (itSprites != _sprites.end())
    {
        //Feedback to tell us it already found the file path's resources loaded in
        std::cout << "*> File path exists (" << filePath << "). Setting referenced sprite with existing image and texture..." << std::endl;
        //Set the referenced sprite's image to the found key's value's image
        sprite.image = (*itSprites).second.image;
        //Set the referenced sprite's texture to the found key's value's texture
        sprite.texture = (*itSprites).second.texture;
    }
    else
    {
        //Feedback to tell us it didn't find the file path's loaded resources; thus, creates a new sprite object
        std::cout << "*> File path does not yet exist (" << filePath << "). Initialising new image and texture for referenced sprite..." << std::endl;
        //LoadImage takes in a C-string, so convert the std::string file path and initialise the image data
        sprite.image = LoadImage((filePath).c_str());
        //Initialise the texture from the image
        sprite.texture = LoadTextureFromImage(sprite.image);

        //Insert the new sprite--constructed from the above loaded image and texture data--into the list of sprites for future searches.
        _sprites.emplace(filePath, Sprite{ sprite.image, sprite.texture });;
    }
}

#include "Brick.h"

//Initialise static variables to keep the linker happy
std::vector<Brick*> Brick::_brickList;
int Brick::_brickIDTotal;
Color Brick::_colMap[10] = { GOLD, GRAY, RED, GREEN, YELLOW, ORANGE, WHITE, Color{0, 255, 255, 255}, BLUE, MAGENTA };

Brick::Brick(Vector2 spawn, char inputType) : RectObject(spawn, "brick.png")
{
	//Brick::_image = LoadImage("../resources/brick.png");
	//Brick::_sprite = LoadTextureFromImage(Brick::_image);


	std::cout << "->\tNew Brick!" << std::endl;	//feedback
	Brick::_brickList.push_back(this);	//add the brick to the brick list
	Brick::_brickID = Brick::_brickIDTotal++;	//set this brick's ID to the current brick ID total, then add to the total so the next brick gets a unique ID

	//Brick::_colour = Brick::_colMap[(int)Brick::_brickType];

	Brick::_brickHealth = 1;	//default to 1 unless changed by SetBrickType
	Brick::_colour = Brick::SetBrickType(inputType);

	Brick::_scale = { 0.8f, 0.8f };

	//treat spawn values as grid coordinates relative to the size of the bricks...
	Brick::_pos.x = ((Brick::Size().x) * spawn.x) + _boundaryPtr->x;
	Brick::_pos.y = ((Brick::Size().y) * spawn.y) + _boundaryPtr->y;
}

Brick::~Brick()
{
	//Go through the _brickList vector using an iterator, from start to end
	for (std::vector<Brick*>::iterator itBrick = Brick::_brickList.begin(); itBrick != Brick::_brickList.end(); ++itBrick)
	{
		//if the iterator comes across a class instance whose _brickID is this one's, we found the exact instance we want to remove
		if ((*itBrick)->_brickID == this->_brickID)
		{
			Brick::_brickList.erase(itBrick);	//remove this class instance from the list
			break;	//break from loop, otherwise it will iterate into forbidden territory since we just resized the vector with the above command
		}
	}
}

void Brick::Update(float deltaTime)
{

}

Color Brick::SetBrickType(char inputType)
{
	switch (inputType)
	{
	case 'w':
		return WHITE;

	case 'r':
		return RED;

	case 'y':
		return YELLOW;

	case 'm':
		return MAGENTA;

	case 'b':
		return BLUE;

	case 'g':
		return GREEN;

	case 'o':
		return ORANGE;

	case 'c':
		return Color {0, 255, 255};	//no cyan defined by RayLib for some reason

	case 's':
		Brick::_brickHealth = 3;	//3 hits to destroy
		return LIGHTGRAY;

	case 'G':
		Brick::_brickHealth = -1;	//-1 means invincible
		return GOLD;		//Captial G = Gold, lower-case g = green

	default:
		return BLACK;		//There are no black bricks--if this is returned, something's wrong.
	}
}

void Brick::BrickBallColRes(Ball* ball)
{
	//Get the collision result between this paddle and the referenced ball object
	RectObject::RectColResult colResult = RectCircleCollision(ball);

	switch (colResult)
	{
	case RectObject::RectColResult::Top:
		ball->_direction.y = -abs(ball->_direction.y);     //Set ball's Y direction to point up
		break;

	case RectObject::RectColResult::Bottom:
		ball->_direction.y = abs(ball->_direction.y);     //Set ball's Y direction to point down
		break;

	case RectObject::RectColResult::Left:
		ball->_direction.x = -abs(ball->_direction.x);    //Set ball's X direction to point right
		break;

	case RectObject::RectColResult::Right:
		ball->_direction.x = abs(ball->_direction.x);     //Set ball's X directino to point left
		break;
	}
}

void Brick::Render()
{
	RectObject::Render();
}
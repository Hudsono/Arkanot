#include "Brick.h"
#include "Game.h"

//Initialise static variables to keep the linker happy
std::vector<Brick*> Brick::_brickList;
int Brick::_brickIDTotal;
Color Brick::_colMap[10] = { GOLD, GRAY, RED, GREEN, YELLOW, ORANGE, WHITE, Color{0, 255, 255, 255}, BLUE, MAGENTA };

Brick::Brick(Vector2 levelPos, char inputType) : RectObject(levelPos, "brick.png")
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

	//treat level position values as grid coordinates relative to the size of the bricks to determine its X, Y position...
	//Flip inputted levelPos axis as well...
	Brick::_pos.x = ((Brick::Size().x) * levelPos.y) + _boundaryPtr->x;
	Brick::_pos.y = ((Brick::Size().y) * levelPos.x) + _boundaryPtr->y;

	//Record this brick's level position from the input.
	Brick::_levelPos = levelPos;

	_shineTime = 0;	// Brick is not yet shining.
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
	if (_shineTime > 0)
	{
		_shineTime--;
		_colourAdd = WHITE;
	}
	else
	{
		_colourAdd = BLANK;
	}
}

Color Brick::SetBrickType(char inputType)
{
	switch (inputType)
	{
	case 'w':
		_brickType = BrickTypes::White;
		return WHITE;

	case 'r':
		_brickType = BrickTypes::Red;
		return RED;

	case 'y':
		_brickType = BrickTypes::Yellow;
		return YELLOW;

	case 'm':
		_brickType = BrickTypes::Magenta;
		return MAGENTA;

	case 'b':
		_brickType = BrickTypes::Blue;
		return BLUE;

	case 'g':
		_brickType = BrickTypes::Green;
		return GREEN;

	case 'o':
		_brickType = BrickTypes::Orange;
		return ORANGE;

	case 'c':
		_brickType = BrickTypes::Cyan;
		return Color { 0, 255, 255, 255 };	// No cyan defined by RayLib for some reason

	case 's':
		_brickType = BrickTypes::Silver;
		Brick::_brickHealth = 2;	//2 hits to destroy initially
		return LIGHTGRAY;

	case 'G':
		_brickType = BrickTypes::Gold;
		Brick::_brickHealth = -1;	//-1 means invincible
		return GOLD;		//Captial G = Gold, lower-case g = green

	default:
		_brickType = BrickTypes::Black;
		return BLACK;		//There are no black bricks--if this is returned, something's wrong.
	}
}

//void Brick::BrickBallColRes(Ball* ball)
//{
//	//Get the collision result between this paddle and the referenced ball object
//	RectObject::RectColResult colResult = RectCircleCollision(ball);
//
//	//Bounce the referenced ball back in the appropriate direction, given the above rect-circle collision
//	switch (colResult)
//	{
//	case RectObject::RectColResult::Top:
//		ball->_direction.y = -abs(ball->_direction.y);     //Set ball's Y direction to point up
//		break;
//
//	case RectObject::RectColResult::Bottom:
//		ball->_direction.y = abs(ball->_direction.y);     //Set ball's Y direction to point down
//		break;
//
//	case RectObject::RectColResult::Left:
//		ball->_direction.x = -abs(ball->_direction.x);    //Set ball's X direction to point right
//		break;
//
//	case RectObject::RectColResult::Right:
//		ball->_direction.x = abs(ball->_direction.x);     //Set ball's X directino to point left
//		break;
//	}
//
//	//Check this brick's health
//	//Gold bricks cannot break; don't bother with them
//	if (_brickType != BrickTypes::Gold)
//	{
//		_brickHealth--;
//		if (_brickHealth < 1)
//			Break();
//	}
//}

void Brick::ImpactBrick()
{
	cout << "Brick Impacted! ID = " << _brickID << endl;
	//cout << "Brick type = " << _brickType << endl;

	// If 1 point of health taken from this brick is 0, break it.
	if (--_brickHealth == 0)
		Break();

	// If this brick is Gold or Silver, shine it
	if (_brickType == Brick::BrickTypes::Gold || _brickType == Brick::BrickTypes::Silver)
	{
		Shine();
	}
}

void Brick::Shine()
{
	_shineTime = 5;
}

void Brick::Break()
{
	//remove this brick from the level map
	//level map = 2D array of Brick pointers.
	//Here, delete that pointer/set it to something that means "there's no brick here"
	//std::cout << "BROKEN BRICK!" << endl;

	//Remove this brick from the list and destroy it.
	Brick::~Brick();

	//Check if there are no more bricks...
	//TODO: let the Game object handle this logic (so we can pause it and move on), and check if there are no /non-gold/ bricks present.
	if (_brickList.size() == 0)
	{
		cout << "------------------------------------YOU WIN!!!-------------------------------------" << endl;
		Game::_paused = true;
	}
}

void Brick::Render()
{
	RectObject::Render();
}
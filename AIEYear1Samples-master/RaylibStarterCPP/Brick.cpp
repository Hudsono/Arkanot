#include "Brick.h"
#include "Game.h"
#include "Powerup.h"

//Initialise static variables to keep the linker happy
std::vector<Brick*> Brick::_brickList;
int Brick::_brickIDTotal;
int Brick::_numBricksToDestroy;
//Color Brick::_colMap[10] = { GOLD, GRAY, RED, GREEN, YELLOW, ORANGE, WHITE, Color{0, 255, 255, 255}, BLUE, MAGENTA };

// Constructor here has a condition based on the input type--if it's 's' or 'G' (Silver or Gold), then use brickM.png, otherwise use brick.png
// I.e., metallic bricks that don't destroy in 1 hit appear different
Brick::Brick(Vector2 levelPos, char inputType) : RectObject(levelPos, (inputType == 's' || inputType == 'G') ? "brickM.png" : "brick.png")
{
	//Brick::_image = LoadImage("../resources/brick.png");
	//Brick::_sprite = LoadTextureFromImage(Brick::_image);


	std::cout << "->\tNew Brick!" << std::endl;	//feedback
	Brick::_brickList.push_back(this);	//add the brick to the brick list
	Brick::_brickID = Brick::_brickIDTotal++;	//set this brick's ID to the current brick ID total, then add to the total so the next brick gets a unique ID


	//Brick::_colour = Brick::_colMap[(int)Brick::_brickType];

	Brick::_brickHealth = 1;	//default to 1 unless changed by SetBrickType
	Brick::_colour = Brick::SetBrickType(inputType);

	// If the brick we just added is NOT Gold, increment the number of bricks to destroy.
	if (_brickType != BrickTypes::Gold)
		Brick::_numBricksToDestroy++;

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
	// Go through the _brickList vector using an iterator, from start to end
	for (std::vector<Brick*>::iterator itBrick = Brick::_brickList.begin(); itBrick != Brick::_brickList.end(); ++itBrick)
	{
		// If the iterator comes across a class instance whose _brickID is this one's, we found the exact instance we want to remove
		if ((*itBrick)->_brickID == this->_brickID)
		{
			if ((*itBrick)->_brickType != BrickTypes::Gold)
				_numBricksToDestroy--;	// Decrement the number of bricks to destroy, if this is not Gold-coloured.
			Brick::_brickList.erase(itBrick);	// Remove this class instance from the list
			break;	// Break from loop, otherwise it will iterate into forbidden territory since we just resized the vector with the above command
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
	//cout << "Brick Impacted! ID = " << _brickID << endl;
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
	// First, we want to award the player points based on what brick they destroyed...
	// If we destroyed a Silver brick, its score is multiplied by 50 for every 8th level.
	if (Brick::_brickType == Brick::BrickTypes::Silver)
	{
		Game::_score += ((int)Brick::_brickType * (1 + (int)(Game::_levelNum / 8)));
	}
	else
	{
		Game::_score += (int)Brick::_brickType;
		
		// We also throw a random roll to spawn a power-up for non-Silver bricks--spawn these in the centre of the brick.
		int randNum = rand() % 100;

		// 20% chance of a power-up spawning.
		if (randNum < 20)
			new Powerup({ Brick::_pos.x + (Brick::Size().x / 2), Brick::_pos.y + (Brick::Size().y / 2)});
	}

	// Remove this brick from the list and destroy it.
	Brick::~Brick();
}

bool Brick::CheckBrickWin()
{
	// Below method works, but the loop is more costly than just keeping track of the bricks as they're created and destroyed...
	//int numBricksDestroy = 0;
	//for (int i = 0; i < Brick::_brickList.size(); i++) if (Brick::_brickList[i]->_brickType != Brick::BrickTypes::Gold)
	//{
	//	numBricksDestroy++;
	//}

	// Returns True if there are no more destructible bricks present.
	return Brick::_numBricksToDestroy == 0;
}

void Brick::Render()
{
	RectObject::Render();
}
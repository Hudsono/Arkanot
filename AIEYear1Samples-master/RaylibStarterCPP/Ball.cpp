#include "Ball.h"

//Initialise static variables to keep the linker happy
std::vector<Ball*> Ball::_ballList;
int Ball::_ballIDTotal;

Ball::Ball(Vector2 spawn, Vector2 direction) : CircleObject(spawn, "ball.png")
{
	std::cout << "->\tNew Ball!" << std::endl;	//feedback
	Ball::_ballID = Ball::_ballIDTotal++;	//set this ball's ID to the current ball ID total, then add to the total so the next ball gets a unique ID

	Ball::_direction = direction;
	Ball::_speed = 8;
	Ball::_maxSpeed = 12;
	Ball::_colour = Color {255, 255, 255, 255};

	Ball::_velocity = 1;
	Ball::_stuckOffset = 0;

	Ball::_radius = 8;
	Ball::_prevPos = {NAN, NAN};

	Ball::_centreSprite = true;	//force the sprite to render offset to the centre of the object

	Ball::_stuckPaddle = nullptr;

	Ball::_ballList.push_back(this);	//add the ball to the ball list
}

Ball::~Ball()
{
	//Delete pointers to unreserve their data, and set their address to nothing should they still be called after
	delete _stuckPaddle;
	_stuckPaddle = nullptr;

	//Go through the _ballList vector using an iterator, from start to end
	for (std::vector<Ball*>::iterator itBall = Ball::_ballList.begin(); itBall != Ball::_ballList.end(); ++itBall)
	{
		//if the iterator comes across a class instance whose _ballID is this one's, we found the exact instance we want to remove
		if ((*itBall)->_ballID == _ballID)
		{
			Ball::_ballList.erase(itBall);	//remove this class instance from the list
	
			cout << "Deleted ball #" << _ballID << endl;
			cout << "Ball list size = " << Ball::_ballList.size() << endl;
	
			break;	//break from loop, otherwise it will iterate into forbidden territory since we just resized the vector with the above command
		}
	}
}

void Ball::Update(float deltaTime)
{
	Ball::_numCol = 0;	//reset number of collisions

	//If the ball isn't stuck on a paddle, move it normally
	if (Ball::_stuckPaddle == nullptr)
	{
		Ball::MoveBall(deltaTime);
	}

}

void Ball::StickToPaddle(Paddle* paddle)
{
	//Stick this ball to the given paddle
	Ball::_stuckPaddle = paddle;

	//Add this ball to the given paddle's list of balls stuck to it
	paddle->_stuckBalls.push_back(this);

	//Reset the paddle's stuck timer to its max value upon sticking
	paddle->_stuckTimer = Paddle::_stuckTimerMax;

	//Set ball to yellow to signify it's caught
	Ball::_colour = Color{ 255, 255, 0, 255 };	

	//Reset some values
	Ball::_scale = { 1, 1 };

	//Set the stuck offset
	Ball::_stuckOffset = Ball::_pos.x - paddle->_pos.x;

	//Set this ball's previous position to NAN so that they won't immediately collide with each-other when released--getting stuck
	Ball::_prevPos = { NAN, NAN };
}

void Ball::Disrupt()
{
	//new Ball({_pos.x + rand(), _pos.y + rand()}, { _direction.x + ((rand() % 2) - 1), _direction.y + ((rand() % 2) - 1) });
	new Ball({ _pos.x + 5, _pos.y + 10 }, { 0.5f, 0.5f });
}

void Ball::BallRectColRes(RectObject* rectobj)
{
	//Get the collision result from the given rectobject
	RectObject::RectColResult colResult = rectobj->RectCircleCollision(this);

	//Act on the collision based on what side was hit, if any
	switch (colResult)
	{
	case RectObject::RectColResult::Top:
		this->_direction.y = -abs(this->_direction.y);    //Set ball's Y direction to point up
		break;

	case RectObject::RectColResult::Bottom:
		this->_direction.y = abs(this->_direction.y);     //Set ball's Y direction to point down
		break;

	case RectObject::RectColResult::Left:
		this->_direction.x = -abs(this->_direction.x);    //Set ball's X direction to point right
		break;

	case RectObject::RectColResult::Right:
		this->_direction.x = abs(this->_direction.x);     //Set ball's X directino to point left
		break;

	default:
		break;
	}
}

void Ball::MoveBall(float deltaTime)
{
	Ball::AddPos({ _direction.x * _speed, _direction.y * _speed });

	//ball reaches bottom of screen = delete the ball
	//or, ball reaches bottom of boundary = delete the ball
	//or, ball's position is not a number (an edge case issue in spawning balls from other balls en masse using the Disrupt function)
	//if this is the last ball, remove a life from the player and give them another ball
	//if this is their last life, GameOver();
	if (Ball::_pos.y + Ball::_radius >= GetScreenHeight() || Ball::_pos.y + Ball::_radius >= Ball::_boundaryPtr->y + Ball::_boundaryPtr->height || Helper::isNaNVector(Ball::_pos))
	{
		cout << "DELETE" << endl;
		delete this;
		//Ball::~Ball();
	}

	//bounce ball off walls
	//top/bottom of screen
	if (Ball::_pos.y - Ball::_radius <= 0 || Ball::_pos.y + Ball::_radius >= GetScreenHeight())
	{
		Ball::_direction = { Ball::_direction.x, Ball::_direction.y * -1 };	//invert Y axis
		Ball::_numCol++;
	}
	//left/right of screen
	if (Ball::_pos.x - Ball::_radius <= 0 || Ball::_pos.x + Ball::_radius >= GetScreenWidth())
	{
		Ball::_direction = { Ball::_direction.x * -1, Ball::_direction.y };	//invert X axis
		Ball::_numCol++;
		//Ball::_direction = { Ball::_direction.x * -1.1f, Ball::_direction.y };	//invert X axis --proof-of-concept, half the horizontal speed is lost here
		//Helper::Normalise(Ball::_direction);	//normalise the direction so the X and Y direction is always = 1 aka don't change its speed.
	}

	//bounce ball off boundary
	//top/bottom of boundary
	if (Ball::_pos.y - Ball::_radius <= Ball::_boundaryPtr->y || Ball::_pos.y + Ball::_radius >= Ball::_boundaryPtr->y + Ball::_boundaryPtr->height)
	{
		Ball::_direction = { Ball::_direction.x, Ball::_direction.y * -1 };	//invert Y axis
		Ball::_numCol++;
	}
	//left/right of screen
	if (Ball::_pos.x - Ball::_radius <= Ball::_boundaryPtr->x || Ball::_pos.x + Ball::_radius >= Ball::_boundaryPtr->x + Ball::_boundaryPtr->width)
	{
		Ball::_direction = { Ball::_direction.x * -1, Ball::_direction.y };	//invert X axis
		Ball::_numCol++;
		//Ball::_direction = { Ball::_direction.x * -1.1f, Ball::_direction.y };	//invert X axis --proof-of-concept, half the horizontal speed is lost here
		//Helper::Normalise(Ball::_direction);	//normalise the direction so the X and Y direction is always = 1 aka don't change its speed.
	}

	Ball::_velocity = (abs(Ball::_prevPos.x - Ball::_pos.x) + abs(Ball::_prevPos.y - Ball::_pos.y)) / 2;
	//cout << _velocity << endl;

	//Change the ball colour based on how fast it's going
	// We take the ball's velocity and -4 as 4 appears to be the base velocity
	// We divide this velocity output by 6 to define a range. Now, 4-10 is the ball's colour-changing range (4 = green, 10 = red)
	// * 255 so it's within the 0-255 colour range
	// Clamp it so it doesn't set a negative colour or above 255 (which reverts back to 0)
	Ball::_colour.r = Helper::Clamp(abs(((Ball::_velocity - 4) / 6) * 255), 0, 255);
	// The green channel is set to the lack of the red channel
	Ball::_colour.g = 255 - Ball::_colour.r;

	// Elongate the ball sprite based on how fast it's going--cartoon style
	float stretch = 30;
	// First, get a normalised version of the ball's direction so we can calculate its angle of travel
	Vector2 normDir = Helper::Normalised(Ball::_direction);
	float dirAngle = atan2f(normDir.x, normDir.y) * RAD2DEG;	//Get the angle of travel in degrees
	Ball::_angleDeg = -dirAngle;	//Set the object's angle to the ball's travelling angle
	Ball::_scale.y = 1 + ((Ball::_velocity / stretch));	//Set the ball's Y scale based on how fast it's going, proportional to the amount of stretch
	//Ball::_spriteOffset = { -((float)Ball::_image.width / 2), -((float)Ball::_image.height / 2) - Ball::_scale.y / stretch };
}

void Ball::Render()
{
	GameObject::Render();
}


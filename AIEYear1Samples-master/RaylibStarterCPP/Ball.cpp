#include "Ball.h"

void Ball()
{
	cout << "New Ball!" << endl;
	_numBalls++;
}

void Ball::Update(float deltaTime)
{

}

void Ball::Render()
{
	DrawCircle(_pos.x, _pos.y, 5, GREEN);
}


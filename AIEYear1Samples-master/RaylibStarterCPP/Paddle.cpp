#include "Paddle.h"

void Paddle::Update(float deltaTime)
{

}

void Paddle::Render()
{
	DrawRectangle(_pos.x, _pos.y, 50, 50, RED);
}


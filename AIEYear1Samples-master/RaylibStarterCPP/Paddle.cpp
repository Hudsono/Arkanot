#include "Paddle.h"
#include "Ball.h"


Paddle::Paddle(Vector2 spawn) : GameObject(spawn)
{
	Paddle::_speed = 15;

	Paddle::_image = LoadImage("../resources/paddle.png");
	Paddle::_sprite = LoadTextureFromImage(Paddle::_image);
	Paddle::_scale = { 1, 1 };
	
	Paddle::_paddleState = Paddle::PaddleState::Catch;
}

void Paddle::Update(float deltaTime)
{
	//Here, move all balls that are stuck to the paddle.
	//First, check there are any balls stuck in the first place to bother calculating...
	if (!Paddle::_stuckBalls.empty())
	{
		//Ball::SetPos(Vector2{ Ball::_pos.x, Ball::_stuckPaddle->_pos.y });

		//Go through the _stuckBalls vector using an iterator, from start to end
		for (vector<Ball*>::iterator itSBall = Paddle::_stuckBalls.begin(); itSBall != Paddle::_stuckBalls.end(); ++itSBall)
		{
			//Set ball position to paddle's + the ball's X offset + sit a little bit above the paddle
			Ball* ball = (*itSBall);
			ball->SetPos(Vector2{ Paddle::_pos.x + ball->_stuckOffset, Paddle::_pos.y - ball->_size });
		}
	}

}

void Paddle::MoveLeft()
{
    //Move left so long as the paddle isn't beyond the game boundary
    if (Paddle::_pos.x >= GameObject::_boundaryPtr->x)
    {
        Paddle::AddPos(Vector2{ -Paddle::_speed, 0 });
    }
    //If it went beyond the boundary, correct it
    if (Paddle::_pos.x < GameObject::_boundaryPtr->x)
    {
        Paddle::SetPos(Vector2{ GameObject::_boundaryPtr->x, Paddle::_pos.y });
    }
}

void Paddle::MoveRight()
{
	//Move right so long as the paddle isn't beyond the game boundary
	if (Paddle::_pos.x + Paddle::Size().x <= GameObject::_boundaryPtr->x + GameObject::_boundaryPtr->width)
	{
		Paddle::AddPos(Vector2{ Paddle::_speed, 0 });
	}
	//If it went beyond the boundary, correct it
	if (Paddle::_pos.x + Paddle::Size().x > GameObject::_boundaryPtr->x + GameObject::_boundaryPtr->width)
	{
		Paddle::SetPos(Vector2{ GameObject::_boundaryPtr->x + GameObject::_boundaryPtr->width - Paddle::Size().x, Paddle::_pos.y });
	}
}

void Paddle::Fire()
{
	//Fire any balls stuck to the paddle, no matter the paddle's state (could have changed state or start with a stuck ball for example)
	if (!Paddle::_stuckBalls.empty())
	{
		//Ball::SetPos(Vector2{ Ball::_pos.x, Ball::_stuckPaddle->_pos.y });

		//Go through the _stuckBalls vector using an iterator, from start to end
		//Move all balls
		for (vector<Ball*>::iterator itSBall = Paddle::_stuckBalls.begin(); itSBall != Paddle::_stuckBalls.end(); ++itSBall)
		{
			//Fire each ball
			Ball* ball = (*itSBall);
			ball->SetPos(Vector2{ ball->_pos.x, ball->_pos.y - 10 });	//Lift the ball a tad up from the paddle so it doesn't intersect it immediately after firing
			ball->_prevPos = ball->_pos;	//Set this ball's previous position to now.
			
			//Set direction to up
			ball->_direction = { 0, -1 };
			//calculate how much X direction bias based on how close the ball is to the centre of the paddle
			float sharpness = -((Paddle::_pos.x + (Paddle::Size().x / 2)) - ball->_pos.x) / (Paddle::Size().x / 2);

			//apply sharpness to direction
			ball->_direction.x = sharpness;

			ball->_stuckPaddle = nullptr;	//Set this ball to not be stuck to any paddle anymore
		}

		//Clear the vector of balls since we fired all of them off. Don't need to target-and-remove them individually.
		Paddle::_stuckBalls.clear();
	}
}

void Paddle::Render()
{
	GameObject::Render();
}


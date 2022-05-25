#include "Paddle.h"
#include "Ball.h"

int Paddle::_stuckTimerMax = 200;

Paddle::Paddle(Vector2 spawn) : RectObject(spawn, "paddle.png")
{
	Paddle::_speed = 15;

	//Paddle::_image = LoadImage("../resources/paddle.png");
	//Paddle::_sprite = LoadTextureFromImage(Paddle::_image);
	
	Paddle::_paddleState = Paddle::PaddleState::Catch;
	Paddle::_stuckTimer = 0;

	Paddle::ResetPaddle();
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
			ball->SetPos(Vector2{ Paddle::_pos.x + ball->_stuckOffset, Paddle::_pos.y - ball->_radius });
		}

		//Count down the stuck timer if it's above 0
		//If 0, Fire all balls
		if (Paddle::_stuckTimer > 0)
			Paddle::_stuckTimer--;
		else
			Paddle::Fire();
	}

}

void Paddle::MoveLeft()
{
    //Move left so long as the paddle isn't beyond the game boundary
    if (Paddle::_pos.x >= RectObject::_boundaryPtr->x)
    {
        Paddle::AddPos(Vector2{ -Paddle::_speed, 0 });
    }
    //If it went beyond the boundary, correct it
    if (Paddle::_pos.x < RectObject::_boundaryPtr->x)
    {
        Paddle::SetPos(Vector2{ RectObject::_boundaryPtr->x, Paddle::_pos.y });
    }
}

void Paddle::MoveRight()
{
	//Move right so long as the paddle isn't beyond the game boundary
	if (Paddle::_pos.x + Paddle::Size().x <= RectObject::_boundaryPtr->x + RectObject::_boundaryPtr->width)
	{
		Paddle::AddPos(Vector2{ Paddle::_speed, 0 });
	}
	//If it went beyond the boundary, correct it
	if (Paddle::_pos.x + Paddle::Size().x > RectObject::_boundaryPtr->x + RectObject::_boundaryPtr->width)
	{
		Paddle::SetPos(Vector2{ RectObject::_boundaryPtr->x + RectObject::_boundaryPtr->width - Paddle::Size().x, Paddle::_pos.y });
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
			//ball->_prevPos = ball->_pos;	//Set this ball's previous position to now.
			
			//Set direction to up
			ball->_direction = { 0, -1 };
			//calculate how much X direction bias based on how close the ball is to the centre of the paddle
			float sharpness = -((Paddle::_pos.x + (Paddle::Size().x / 2)) - ball->_pos.x) / (Paddle::Size().x / 2);

			//Exclusively clamp the sharpness so that it doesn't always shoot out directly upwards/with little X movement. That's boring.
			sharpness = Helper::ClampOut(sharpness, -0.3f, 0.3f);

			//apply sharpness to direction
			ball->_direction.x = sharpness;

			ball->_stuckPaddle = nullptr;	//Set this ball to not be stuck to any paddle anymore
		}

		//Clear the vector of balls since we fired all of them off. Don't need to target-and-remove them individually.
		Paddle::_stuckBalls.clear();

		//Reset the paddle's _stuckTimer
		Paddle::_stuckTimer = 0;
	}
}

void Paddle::PaddleBallColRes(Ball* ball)
{
	//Get the collision result between this paddle and the referenced ball object
	RectObject::RectColResult colResult = RectCircleCollision(ball);

	//Declare and initialise some variables to use in the switch
	//How C++ combs through switches prevents variable initialisation within
	bool rightHalf = false;
	float sharpness = 0;

	switch (colResult)
	{
	case RectObject::RectColResult::Top:
        //If the ball touches the top of the paddle, execute some gameplay logic to keep the ball's direction predictable and controllable,
        //but also varied.

        //check if the ball landed on the right half of the paddle. For top collision.
        //in Arkanoid, the ball is resolved differently depending on which half of the paddle the ball lands on
        rightHalf = (ball->_pos.x > this->_pos.x + (this->Size().x / 2));

        ball->_direction.y = -abs(ball->_direction.y);    //Set ball's Y direction to point up

            //Reflection (below) proved difficult when colliders were inside each-other (i.e. it would reflect back-and-forth over and over)
            //ball1->_direction.y *= -1;

            //calculate how much X direction bias based on how close the ball is to the centre of the paddle
        sharpness = -((this->_pos.x + (this->Size().x / 2)) - ball->_pos.x) / (this->Size().x / 2);

		//Exclusively clamp the sharpness so that it doesn't always shoot out directly upwards/with little X movement. That's boring.
		sharpness = Helper::ClampOut(sharpness, -0.3f, 0.3f);

        //if on the right, also reflect the X direction
        if (rightHalf)
            ball->_direction.x = sharpness;
        else
            ball->_direction.x += sharpness;

        //normalise direction
        // Actually, in Arkanoid, it doesn't look like the direction is normalised on purpose
        // This way, the ball speeds up when hit on the paddle edges, and slows down when hitting the centre of the paddle
        // AKA varied gameplay and controlled skill.
        //Helper::Normalise(ball1->_direction);

        // Make sure the ball's vertical direction doesn't become too shallow
        // Otherwise, the ball can get stuck in the level without much player input
        // Arkanoid also seems to impose this limit

        // If the Y direction is in the exclusive range -0.5 to 0.5, the ball is moving too slow on the Y axis
        // So, clamp the Y direction outside of this range (inclusive)
        if (ball->_direction.y > -0.5f && ball->_direction.y < 0.5)
            ball->_direction.y = Helper::ClampOut(ball->_direction.y, -0.5f, 0.5f);

        // Also impose an X/horizontal direction limit, so that the game is not dead easy with the
        // ball bouncing directly up and down.
        // Again, Arkanoid appears to impose this limit.

        // If the X direction is in the exclusive range -0.5 to 0.5, the ball is moving too slow on the X axis
        // So, clamp the X direction outside of this range (inclusive)
        if (ball->_direction.x > -0.5f && ball->_direction.x < 0.5)
            ball->_direction.x = Helper::ClampOut(ball->_direction.x, -0.5f, 0.5f);


        //If the paddle is in "catch" mode, then attach the ball to the paddle if it hits the top
        if (this->_paddleState == Paddle::PaddleState::Catch)
        {
            ball->StickToPaddle(this);
        }
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

	default:
		break;
	}
}

void Paddle::ResetPaddle()
{
	//Reset the paddle's state.
	this->_paddleState = Paddle::PaddleState::Normal;

	//Set paddle position to middle of screen. Offsets by the width of the paddle.
	SetPos({ (GetScreenWidth() / 2) - (this->Size().x / 2), _pos.y });

	//Spawn a starting ball slightly off-centre on top of this paddle and stick it to this paddle.
	Ball* temp = new Ball({ _pos.x + (Size().x / 1.8f), _pos.y}, {0.5, -0.5});
	temp->StickToPaddle(this);
}

void Paddle::Render()
{
	RectObject::Render();
}


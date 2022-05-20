#include "RectObject.h"

RectObject::RectObject(Vector2 spawn, const char* textureFile) : GameObject(spawn, textureFile)
{

}

RectObject::~RectObject()
{
}

void RectObject::RenderDebug()
{
	DrawRectangleLines(GameObject::_pos.x, GameObject::_pos.y, GameObject::Size().x, GameObject::Size().y, BLACK);	//Draw object bounds as a black rectangle
	GameObject::RenderDebug();
}

RectObject::RectColResult RectObject::RectCircleCollision(CircleObject* circle)
{
	RectColResult colResult = RectObject::RectColResult::None;	//default to no collision made

    //Check against the paddle here
    //Clamp point inside the paddle
    Vector2 clampVal = { Helper::Clamp(circle->_pos.x, this->_pos.x, this->_pos.x + this->Size().x), Helper::Clamp(circle->_pos.y, this->_pos.y, this->_pos.y + this->Size().y) };

    //Get difference between the clamped paddle point and the ball
    Vector2 paddleBallDelta = { clampVal.x - circle->_pos.x, clampVal.y - circle->_pos.y };

    //determine if the ball collided with the paddle
    //get the magnitude^2 of ball positions against squared combinations of both balls' radii
    //avoids square root calculations
    bool paddleCollided = Helper::Dot(paddleBallDelta, paddleBallDelta) <= ((circle->_radius) * (circle->_radius));
    
    //Variables to tell us which side the circle collided with, if any...
    bool touchU = false;
    bool touchD = false;
    bool touchL = false;
    bool touchR = false;

    //If the ball indeed collided with the paddle somehow...
    if (paddleCollided)
    {
        //determine which face the ball touched the paddle
        touchU = (circle->_pos.y < this->_pos.y);
        touchD = (circle->_pos.y > this->_pos.y + this->Size().y);
        touchL = (circle->_pos.x < this->_pos.x);
        touchR = (circle->_pos.x > this->_pos.x + this->Size().x);

        //cout << "--------" << endl;
        //cout << touchU << endl;
        //cout << touchD << endl;
        //cout << touchL << endl;
        //cout << touchR << endl;
        //cout << "--------" << endl;

        // Make one axis win!
        // We only want one axis to respond, otherwise collision resolution makes no sense
        if ((touchU || touchD) && (touchL || touchR))
        {
            //if there's a bigger difference on the X axis, then choose to report X axis collision
            //Otherwise, if there's a bigger difference on the Y axis, then choose to report Y axis collision
            if (abs(paddleBallDelta.x) > abs(paddleBallDelta.y))
            {
                touchU = false;
                touchD = false;
            }
            else
            {
                touchL = false;
                touchR = false;
            }
        }
    }

    //Set the return value appropriate to the active bool:
    if (touchU)
        colResult = RectObject::RectColResult::Top;
    else if (touchD)
        colResult = RectObject::RectColResult::Bottom;
    else if (touchL)
        colResult = RectObject::RectColResult::Left;
    else if (touchR)
        colResult = RectObject::RectColResult::Right;


	return colResult;
}

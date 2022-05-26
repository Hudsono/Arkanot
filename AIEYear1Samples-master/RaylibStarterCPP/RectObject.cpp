#include "RectObject.h"

RectObject::RectObject(Vector2 spawn, const char* textureFile) : GameObject(spawn, textureFile)
{

}

RectObject::~RectObject()
{
}

void RectObject::RenderDebug()
{
	DrawRectangleLines(GameObject::_pos.x, GameObject::_pos.y, GameObject::Size().x, GameObject::Size().y, BLACK);	// Draw object bounds as a black rectangle
	GameObject::RenderDebug();
}

RectObject::RectColResults RectObject::RectCircleCollision(CircleObject* circle)
{
	RectColResults colResult = RectObject::RectColResults::None;	// Default to no collision made

    // Check against the paddle here
    // Clamp point inside the paddle
    Vector2 clampVal = { Helper::Clamp(circle->_pos.x, this->_pos.x, this->_pos.x + this->Size().x), Helper::Clamp(circle->_pos.y, this->_pos.y, this->_pos.y + this->Size().y) };

    // Get difference between the clamped paddle point and the ball
    Vector2 paddleBallDelta = { clampVal.x - circle->_pos.x, clampVal.y - circle->_pos.y };

    // Determine if the ball collided with the paddle
    // Get the magnitude^2 of ball positions against squared combinations of both balls' radii
    // Avoids square root calculations
    bool paddleCollided = Helper::Dot(paddleBallDelta, paddleBallDelta) <= ((circle->_radius) * (circle->_radius));
    
    // Variables to tell us which side the circle collided with, if any...
    bool touchU = false;
    bool touchD = false;
    bool touchL = false;
    bool touchR = false;

    // If the ball indeed collided with the paddle somehow...
    if (paddleCollided)
    {
        // Determine which face the ball touched the paddle
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
            //if (abs(paddleBallDelta.x) > abs(paddleBallDelta.y))
            //{
            //    touchU = false;
            //    touchD = false;
            //}
            //else
            //{
                touchL = false;
                touchR = false;
            //}
        }
    }

    // Set the return value appropriate to the active bool:
    if (touchU)
        colResult = RectObject::RectColResults::Top;
    else if (touchD)
        colResult = RectObject::RectColResults::Bottom;
    else if (touchL)
        colResult = RectObject::RectColResults::Left;
    else if (touchR)
        colResult = RectObject::RectColResults::Right;


	return colResult;
}

bool RectObject::RectRectCollision(RectObject* otherRect)
{
    // Check if the rectangles have overlapped each-other in any way
    // Start with the X axis, accounting for each RectObject's width
    bool xColR = ((RectObject::_pos.x + RectObject::Size().x >= otherRect->_pos.x) && (RectObject::_pos.x + RectObject::Size().x <= otherRect->_pos.x + otherRect->Size().x));
    bool xColL = ((RectObject::_pos.x >= otherRect->_pos.x) && (RectObject::_pos.x <= otherRect->_pos.x + otherRect->Size().x));

    // This check is if /this/ object is wider than the given rectObj and it happens to intersect on the middle
    bool xColM = (otherRect->_pos.x >= this->_pos.x && otherRect->_pos.x + otherRect->Size().x <= this->_pos.x + this->Size().x);
   
    // Then check the Y axis, accounting for each RectObject's height
    bool yColD = (RectObject::_pos.y + RectObject::Size().y >= otherRect->_pos.y && RectObject::_pos.y + RectObject::Size().y <= otherRect->_pos.y + otherRect->Size().y);
    bool yColU = (RectObject::_pos.y >= otherRect->_pos.y && RectObject::_pos.y <= otherRect->_pos.y + otherRect->Size().y);

    // This check is if /this/ object is taller than the given rectObj and it happens to intersect on the middle
    bool yColM = (otherRect->_pos.y >= this->_pos.y && otherRect->_pos.y + otherRect->Size().y <= this->_pos.y + this->Size().y);

    // If there's any kind of intersection present on both axes, we have a collision.
    return ( (xColR || xColL || xColM) && (yColD || yColU || yColM) );
}
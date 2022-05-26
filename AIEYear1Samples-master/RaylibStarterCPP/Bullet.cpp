#include "Bullet.h"
#include "Brick.h"

//Initialise static variables to keep the linker happy
std::vector<Bullet*> Bullet::_bulletList;
int Bullet::_bulletIDTotal;

Bullet::Bullet(Vector2 spawn) : RectObject(spawn, "ball.png")
{
	std::cout << "->\tNew Bullet!" << std::endl;	// Feedback
	Bullet::_bulletID = Bullet::_bulletIDTotal++;	// Set this bullet's ID to the current bullet ID total, then add to the total so the next bullet gets a unique ID

	Bullet::_speed = 15;	// Speed of the bullet
	Bullet::_colour = { 233, 255, 107, 255 };	// Yellow-ish bullets
	Bullet::_scale = { 0.4, 2 };	// Thin, tall bullets

	//Bullet::_centreSprite = true;	// Force the sprite to render offset to the centre of the object

	Bullet::_pos.x -= (Bullet::Size().x / 2);	// Places the bullet in the centre of its spawn area.

	Bullet::_bulletList.push_back(this);	// Add this bullet to the bullet object list
}

Bullet::~Bullet()
{
	// Go through the _bulletList vector using an iterator, from start to end
	for (std::vector<Bullet*>::iterator itBullet = Bullet::_bulletList.begin(); itBullet != Bullet::_bulletList.end(); ++itBullet)
	{
		// If the iterator comes across a class instance whose _bulletID is this one's, we found the exact instance we want to remove
		if ((*itBullet)->_bulletID == _bulletID)
		{
			Bullet::_bulletList[itBullet - Bullet::_bulletList.begin()] = nullptr;	// Set the pointer in the list of this object to nullptr
			Bullet::_bulletList.erase(itBullet);	// Remove this class instance from the list

			break;	// Break from loop, otherwise it will iterate into forbidden territory since we just resized the vector with the above command
		}
	}
}

void Bullet::BulletBrickCol(Brick* brick)
{
	// Booleans for projection...
	bool xColR = (RectObject::_pos.x + RectObject::Size().x >= brick->_pos.x && RectObject::_pos.x + RectObject::Size().x <= brick->_pos.x + brick->Size().x);
	bool xColL = (RectObject::_pos.x >= brick->_pos.x && RectObject::_pos.x <= brick->_pos.x + brick->Size().x);

	// If this bullet indeed collided with the given brick, destroy the bullet and impact the brick.
	if (Bullet::RectRectCollision(brick))
	{
		Bullet::~Bullet();
		brick->ImpactBrick();
	}
	// Here, check if a projection of the bullet's next move will collide with a brick (only need to test on the Y axis). Prevent phasing through.
	else if ((Bullet::_pos.y - Bullet::_speed <= brick->_pos.y + brick->Size().y) && (xColR || xColL))
	{
		// If the projection does go above the brick (i.e. it will collide in the next frame), place the bullet just below this brick.
		// This prevents bullets from destroying a brick above due to the bullet moving too fast through the brick in a single frame.
		Bullet::SetPos({ Bullet::_pos.x, brick->_pos.y + brick->Size().y + 5 });
	}
}

void Bullet::Render()
{
	RectObject::Render();
}

void Bullet::Update(float deltaTime)
{
	// Move the bullet upwards based on its _speed variable.
	AddPos({ 0, -Bullet::_speed });

	// Delete the bullet if it's no longer visible; above the screen.
	if (_pos.y + Size().y < 0)
		Bullet::~Bullet();
}
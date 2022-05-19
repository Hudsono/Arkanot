#include "CircleObject.h"

CircleObject::CircleObject(Vector2 spawn, const char* textureFile) : GameObject(spawn, textureFile)
{

}

CircleObject::~CircleObject()
{

}

void CircleObject::RenderDebug()
{
	DrawCircleLines(_pos.x, _pos.y, _radius, BLACK);
	GameObject::RenderDebug();
}
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
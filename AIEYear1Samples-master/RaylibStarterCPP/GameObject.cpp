#include "GameObject.h"
#include <iostream>

using namespace std;

void GameObject::Update(float deltaTime)
{
	//cout << "Timer = " << deltaTime << endl;
}

void GameObject::Render()
{
	//DrawLine(GetScreenWidth() / 2, GetScreenHeight() / 2, GetMouseX(), GetMouseY(), GREEN);
}

void GameObject::SetPos(Vector2 pos)
{
	_pos.x = pos.x;
	_pos.y = pos.y;
}

void GameObject::AddPos(Vector2 pos)
{
	_pos.x += pos.x;
	_pos.y += pos.y;
}
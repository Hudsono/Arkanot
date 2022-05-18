#include "GameObject.h"
#include <iostream>
#include "Game.h"

using namespace std;

Rectangle* GameObject::_boundaryPtr;

GameObject::GameObject(Vector2 spawn)
{
	//initially set game object's position to the defined spawn location
	_pos = spawn;

	//default initialised values
	_speed = 0;
	_maxSpeed = 1;
	_direction = {0, 0};
	_numCol = 0;
	_colour = WHITE;	//White tint by deafult
	_spriteOffset = { 0, 0 };
	_centreSprite = false;
	_scale = { 1, 1 };
	_angleDeg = 0;

	_image = LoadImage("../resources/none.png");
	_sprite = LoadTextureFromImage(_image);

	_size = { (float)_image.width * _scale.x, (float)_image.height * _scale.y };



	//feedback
	cout << "New GameObject!" << endl;
}

//GameObject::GameObject()
//{
//	//default constructor
//		//initially set game object's position to the defined spawn location
//	_pos = { 0, 0 };
//
//	//default initialised values
//	_speed = 0;
//	_direction = { 0, 0 };
//
//	//feedback
//	cout << "New GameObject but bad!" << endl;
//}

GameObject::~GameObject()
{

}

Vector2 GameObject::Size()
{
	return Vector2 { (float)GameObject::_image.width * (float)GameObject::_scale.x, (float)GameObject::_image.height * (float)GameObject::_scale.y };
}

void GameObject::Update(float deltaTime)
{
	//cout << "Timer = " << deltaTime << endl;
}

void GameObject::Render()
{
	//DrawLine(GetScreenWidth() / 2, GetScreenHeight() / 2, GetMouseX(), GetMouseY(), GREEN);

	//If specified, automatically set the offset to the centre of the image.
	if (GameObject::_centreSprite)
		GameObject::_spriteOffset = { -((float)GameObject::_image.width / 2), -((float)GameObject::_image.height / 2) };

	//Render the texture at the game object's position, offset by any sprite offset specified.
	//DrawTextureEx(GameObject::_sprite, Vector2 { GameObject::_pos.x + GameObject::_spriteOffset.x, GameObject::_pos.y + GameObject::_spriteOffset.y }, GameObject::_angleDeg, 1, GameObject::_colour);
	DrawTexturePro
	(
		GameObject::_sprite, 
		Rectangle
		{ 
			0, 
			0, 
			(float)GameObject::_image.width, 
			(float)GameObject::_image.height 
		},
		Rectangle
		{ 
			GameObject::_pos.x, 
			GameObject::_pos.y, 
			GameObject::_image.width * GameObject::_scale.x, 
			GameObject::_image.height * GameObject::_scale.y 
		},
		Vector2 { -GameObject::_spriteOffset.x, -GameObject::_spriteOffset.y },
		GameObject::_angleDeg, 
		GameObject::_colour
	);

	//If we're in debug mode, draw the sprite's origin/hot spot as a red dot, and the sprite's bounds as a black rectangle
	if (Game::_debugMode)
	{
		DrawCircle(GameObject::_pos.x, GameObject::_pos.y, 2, RED);
		DrawRectangleLines(GameObject::_pos.x, GameObject::_pos.y, GameObject::Size().x, GameObject::Size().y, BLACK);
	}

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
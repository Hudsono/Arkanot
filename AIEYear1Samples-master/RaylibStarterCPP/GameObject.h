#pragma once
#include <iostream>
#include "raylib.h"

using namespace std;

class GameObject
{
protected:
	Vector2 _pos;
	float _speed;
	//sprite
	Vector2 _direction;

public:
	GameObject(Vector2 spawn) : _pos (spawn)
	{
		cout << "New GameObject!" << endl;
	}
	virtual void Update(float timer);
	virtual void Render();

	void SetPos(Vector2 pos);
	void AddPos(Vector2 pos);
};


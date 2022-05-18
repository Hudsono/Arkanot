#pragma once
#include <iostream>
#include <vector>
#include "Helper.h"

using namespace std;

class GameObject
{
public:

	//X, Y position of this object.
	Vector2 _pos;

	//Speed of this object.
	float _speed;

	//Maximum speed of this object.
	float _maxSpeed;

	Image _image;			//Image data for the texture.
	Texture2D _sprite;		//Sprite texture.
	Vector2 _spriteOffset;	//Offset of the sprite relative to the object.
	bool _centreSprite;		//Whether to automatically centre the sprite in the object.
	float _angleDeg;		//Angle of the object, in degrees.
	Vector2 _scale;			//Scale modifier of texture.

protected:
	Vector2 _size;			//Reported scale of the object based on texture.

public:
	Vector2 Size();			//Calculates the current size of the object based on the texture image size multiplied by the scale; returns this.

	//X, Y vector direction of this object's movement.
	Vector2 _direction;

	//Number of collisions against this object.
	int _numCol;

	//Colour tint of this object.
	Color _colour;

	//default constructor
	//GameObject();

	//Constructor with spawn position argument.
	GameObject(Vector2 spawn);

	//Destructor
	~GameObject();

	//RayLib update logic for this object.
	virtual void Update(float timer);

	//RayLib rendering graphics for this object.
	virtual void Render();


	//Set the position of this object.
	void SetPos(Vector2 pos);

	//Add relative position to this object.
	void AddPos(Vector2 pos);

	//Points to the boundary rectangle of the game
	static Rectangle* _boundaryPtr;
};


#pragma once
#include <iostream>
#include <vector>
#include "Helper.h"

using namespace std;

class Game;	// Forward declaration so this class is aware that the Game class exists...

class GameObject
{
public:

	// X, Y position of this object.
	Vector2 _pos;

	// Speed of this object.
	float _speed;

	// Maximum speed of this object.
	float _maxSpeed;

	//Image _image;			// Image data for the texture.
	Sprite _sprite;		// Sprite image and texture.
	Vector2 _spriteOffset;	// Offset of the sprite relative to the object.
	bool _centreSprite;		// Whether to automatically centre the sprite in the object.
	float _angleDeg;		// Angle of the object, in degrees.
	Vector2 _scale;			// Scale modifier of texture.

protected:
	Vector2 _size;			// Reported scale of the object based on texture.

public:
	Vector2 Size();			// Calculates the current size of the object based on the texture image size multiplied by the scale; returns this.

	// X, Y vector direction of this object's movement.
	Vector2 _direction;

	// Number of collisions against this object.
	int _numCol;

	// Colour tint of this object.
	Color _colour;

	// Additive colour of this object.
	Color _colourAdd;

	//default constructor
	//GameObject();

	// Constructor with spawn position argument and default texture file name as pink-black checkerboard image.
	GameObject(Vector2 spawn, const char* textureFile = "none.png");

	// Destructor
	~GameObject();

	// RayLib update logic for this object.
	virtual void Update(float timer);

	// RayLib rendering graphics for this object. Accounts for offsets.
	virtual void Render();

	// Draw debugging graphics, if enabled.
	// Seperate from Render so the debug logic need not be repeated elsewhere, and debug graphics can be redefined where needed.
	// Base draws the hotspot only.
	virtual void RenderDebug();


	// Set the position of this object.
	void SetPos(Vector2 pos);

	// Add relative position to this object.
	void AddPos(Vector2 pos);

	// Points to the boundary rectangle of the game
	static Rectangle* _boundaryPtr;

	// Points to the game instance that instantiated it. Essential for GameObjects to call events and send data to-and-from the Game instance.
	static Game* _gamePtr;

	static int _gameObjIDTotal;
	int _gameObjID;
};


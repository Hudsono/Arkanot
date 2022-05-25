#include "GameObject.h"
#include "Game.h"

using namespace std;

// Initialise static variables to keep the linker happy
int GameObject::_gameObjIDTotal;
Rectangle* GameObject::_boundaryPtr;

GameObject::GameObject(Vector2 spawn, const char* textureFile)
{
	_gameObjID = _gameObjIDTotal++;	// Set this ball's ID to the current ball ID total, then add to the total so the next ball gets a unique ID

	_pos = spawn;	// Initially set game object's position to the defined spawn location

	// Default initialised values
	_speed = 0;
	_maxSpeed = 1;
	_direction = {0, 0};
	_numCol = 0;
	_colour = WHITE;		// White tint by deafult
	_colourAdd = BLANK;		// Transparent additive by default
	_spriteOffset = { 0, 0 };
	_centreSprite = false;
	_scale = { 1, 1 };
	_angleDeg = 0;

	//_image = LoadImage("../resources/none.png");	//default checkerboard texture
	//_image = Helper::LoadImageOp(FILE_PATH + "none.png"));
	//_sprite = LoadTextureFromImage(_image);
	
	// Load the image and initialise the texture based on the supplied file path. GameObject children have this defined in their constructors.
	Helper::LoadSprite(textureFile, _sprite);		

	_size = { (float)_sprite.image.width * _scale.x, (float)_sprite.image.height * _scale.y };

	// Feedback
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
	return Vector2 { (float)GameObject::_sprite.image.width * (float)GameObject::_scale.x, (float)GameObject::_sprite.image.height * (float)GameObject::_scale.y };
}

void GameObject::Update(float deltaTime)
{
	//cout << "Timer = " << deltaTime << endl;
}

void GameObject::Render()
{
	//DrawLine(GetScreenWidth() / 2, GetScreenHeight() / 2, GetMouseX(), GetMouseY(), GREEN);

	// If specified, automatically set the offset to the centre of the image.
	if (GameObject::_centreSprite)
		GameObject::_spriteOffset = { -((float)GameObject::_sprite.image.width / 2), -((float)GameObject::_sprite.image.height / 2) };

	// Render the texture at the game object's position, offset by any sprite offset specified.
	//DrawTextureEx(GameObject::_sprite, Vector2 { GameObject::_pos.x + GameObject::_spriteOffset.x, GameObject::_pos.y + GameObject::_spriteOffset.y }, GameObject::_angleDeg, 1, GameObject::_colour);
	DrawTexturePro
	(
		GameObject::_sprite.texture, 
		Rectangle
		{ 
			0, 
			0, 
			(float)GameObject::_sprite.image.width, 
			(float)GameObject::_sprite.image.height 
		},
		Rectangle
		{ 
			GameObject::_pos.x, 
			GameObject::_pos.y, 
			GameObject::_sprite.image.width * GameObject::_scale.x, 
			GameObject::_sprite.image.height * GameObject::_scale.y 
		},
		Vector2 { -GameObject::_spriteOffset.x, -GameObject::_spriteOffset.y },
		GameObject::_angleDeg, 
		{ (unsigned char)((int)_colour.r + _colourAdd.r), (unsigned char)((int)_colour.g + _colourAdd.g), (unsigned char)((int)_colour.b + _colourAdd.b), (unsigned char)((int)_colour.a + _colourAdd.a) }
	);

	// Render debugging graphics if debugging is enabled
	if (Game::_debugMode)
		RenderDebug();

}

void GameObject::RenderDebug()
{
	DrawCircle(GameObject::_pos.x, GameObject::_pos.y, 2, RED);	// Draw origin/hotspot as a red dot
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

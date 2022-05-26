#include "Powerup.h"

//Initialise static variables to keep the linker happy
std::vector<Powerup*> Powerup::_powerupList;
int Powerup::_powerupIDTotal;

Powerup::Powerup(Vector2 spawn) : RectObject(spawn, "brick.png")
{
	std::cout << "->\tNew Power-Up!" << std::endl;	// Feedback
	Powerup::_powerupID = Powerup::_powerupIDTotal++;	// Set this powerup's ID to the current powerup ID total, then add to the total so the next powerup gets a unique ID

	Powerup::_speed = 3;	// Speed of the power-up's descent
	Powerup::_scale = { 0.5, 0.5 };	// Smaller-scaled sprite

	Powerup::_pos.x -= (Powerup::Size().x / 2);	// Places the powerup in the centre of its spawn area.

	Powerup::_powerupList.push_back(this);	// Add this powerup to the powerup object list

	// Randomly select a powerup type upon construction
	int randNum = rand() % 100;		// Random number 0-99
	// Have to go with an if-else statement set--Visual Studio's C++ compiler doesn't support switch statements with number ranges using "x ... y"
	if (randNum < 15)
		Powerup::_powerupType = Powerup::PowerupTypes::Laser;
	else if (randNum < 30)
		Powerup::_powerupType = Powerup::PowerupTypes::Enlarge;
	else if (randNum < 45)
		Powerup::_powerupType = Powerup::PowerupTypes::Catch;
	else if (randNum < 60)
		Powerup::_powerupType = Powerup::PowerupTypes::Slow;
	else if (randNum < 75)
		Powerup::_powerupType = Powerup::PowerupTypes::Break;
	else if (randNum < 90)
		Powerup::_powerupType = Powerup::PowerupTypes::Disruption;
	else
		Powerup::_powerupType = Powerup::PowerupTypes::Life;
	

	// Set the colour based on the type
	switch (Powerup::_powerupType)
	{
	case PowerupTypes::Laser:
		Powerup::_colour = RED;
		break;

	case PowerupTypes::Enlarge:
		Powerup::_colour = BLUE;
		break;

	case PowerupTypes::Catch:
		Powerup::_colour = GREEN;
		break;

	case PowerupTypes::Slow:
		Powerup::_colour = ORANGE;
		break;

	case PowerupTypes::Break:
		Powerup::_colour = MAGENTA;
		break;

	case PowerupTypes::Disruption:
		Powerup::_colour = { 0, 255, 255, 255 };
		break;

	case PowerupTypes::Life:
		Powerup::_colour = GRAY;
		break;
	}
}

Powerup::~Powerup()
{
	// Go through the _powerupList vector using an iterator, from start to end
	for (std::vector<Powerup*>::iterator itPower = Powerup::_powerupList.begin(); itPower != Powerup::_powerupList.end(); ++itPower)
	{
		// If the iterator comes across a class instance whose _bulletID is this one's, we found the exact instance we want to remove
		if ((*itPower)->_powerupID == _powerupID)
		{
			Powerup::_powerupList[itPower - Powerup::_powerupList.begin()] = nullptr;	// Set the pointer in the list of this object to nullptr
			Powerup::_powerupList.erase(itPower);	// Remove this class instance from the list

			break;	// Break from loop, otherwise it will iterate into forbidden territory since we just resized the vector with the above command
		}
	}
}

void Powerup::Render()
{
	RectObject::Render();
}

void Powerup::Update(float deltaTime)
{
	// Move the powerup downwards based on its _speed variable.
	AddPos({ 0, Powerup::_speed });

	// Delete the powerup if it's no longer visible; below the screen.
	if (_pos.y > GetScreenHeight())
		Powerup::~Powerup();
}
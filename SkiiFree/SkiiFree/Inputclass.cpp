#include "Inputclass.h"

Inputclass::Inputclass()
{
}

Inputclass::Inputclass(const Inputclass& _other)
{
}

Inputclass::~Inputclass()
{
}

void Inputclass::initialize()
{
	int i;

	// Init all the keys to be realesed and not pressed
	for (i = 0; i < 256; i++)
	{
		keys[i] = false;
	}

	return;
}

void Inputclass::keyDown(unsigned int _input)
{
	// If a key is pressed
	keys[_input] = true;
	return;
}

void Inputclass::keyUp(unsigned int _input)
{
	// If a key is released
	keys[_input] = false;
	return;
}

bool Inputclass::isKeyDown(unsigned int _key)
{
	// Retun the state of inputed key
	return keys[_key];
}

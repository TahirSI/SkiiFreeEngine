#include "Systemclass.h"

int WINAPI WinMain(
	HINSTANCE _hInstance, 
	HINSTANCE _hPrevInstance,
	PSTR _pScmdline, 
	int _iCmdshow)
{
	SystemClass* system;
	bool result = false;

	// Create the Sytem object
	system = new SystemClass;
	if (!system)
	{
		return 0;
	}

	// Insilizes and run sytem
	result = system->initialize();
	if (result)
	{
		system->run();
	}

	// Sutdown nd releas obejects
	system->shutDown();
	delete system;
	system = 0;

	return 0;
}
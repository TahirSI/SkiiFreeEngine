#include "Systemclass.h"

SystemClass::SystemClass()
{
	input = 0;
	graphics = 0;
}

SystemClass::SystemClass(const SystemClass&)
{
}

SystemClass::~SystemClass()
{
}

bool SystemClass::initialize()
{
	int screen_With = 0, screen_hight = 0;
	bool reasult;

	// Int window
	intWindow(screen_With, screen_hight);

	// Create input
	input = new Inputclass;
	if (!input)
	{
		return false;
	}

	// init input
	input->initialize();

	// create graphics
	graphics = new Graphicsclass;
	if (!graphics)
	{
		return false;
	}

	//int graphics
	reasult = graphics->initialize(screen_With, screen_hight, hwnd);
	if (!reasult)
	{
		return false;
	}

	return true;
}

void SystemClass::shutDown()
{
	// Release graphics
	if (graphics)
	{
		graphics->shutDown();
		delete graphics;
		graphics = 0;
	}

	// Release input
	if (input)
	{
		delete input;
		input = 0;
	}

	// Window sutdown
	shutDownWindow();

	return;
}

void SystemClass::run()
{
	MSG msg;
	bool done = false, result = false;

	// Int messages
	ZeroMemory(&msg, sizeof(MSG));

	// if not quit
	while (!done)
	{
		// Hand le window messages
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If closeing window
		if (msg.message == WM_QUIT)
		{
			// Quit
			done = true;
		}
		else
		{
			// Do frame processing 
			result = frame();
			if (!result)
			{
				// Quit
				done = true;
			}
		}
	}
	return;
}

LRESULT CALLBACK SystemClass::messageHandler(HWND _hwnd, UINT _msg, WPARAM _wparam, LPARAM _lparam)
{
	switch (_msg)
	{
	// If a key is been pressed
	case WM_KEYDOWN:
	{
		input->keyDown((unsigned int)_wparam);
		return 0;
	}

	// ifKey was released
	case WM_KEYUP:
	{
		input->keyUp((unsigned int)_wparam);
		return 0;
	}

	// Other messages taht the prgram won't use 
	default:
		return DefWindowProc(_hwnd, _msg, _wparam, _lparam);
	}
}

bool SystemClass::frame()
{
	bool result;
	
	// If escape is pressed
	if (input->isKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// Proces graphics farme
	result = graphics->frame();
	if (!result)
	{
		return false;
	}

	return true;
}

void SystemClass::intWindow(int& _screen_with, int& _screen_height)
{
	WNDCLASSEX wc;
	DEVMODE dm_screen_setteings;
	int posX = 0, posY = 0;

	// Get a exernal handle to tjis pointer
	aplicationHandle = this;

	// get the inst to the aplication
	hinstance = GetModuleHandle(NULL);

	// Aplication name
	apliaction_name = L"Skii Game";

	// Setup windpw
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = windProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hinstance;
	wc.hIcon = LoadIcon(NULL, IDC_ARROW);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = apliaction_name;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register class
	RegisterClassEx(&wc);

	// Determin the clinent window size
	_screen_with = GetSystemMetrics(SM_CXSCREEN);
	_screen_height = GetSystemMetrics(SM_CYSCREEN);

	// setting the screen setting based on full screen or windowed
	if (FULL_SCREEN)
	{
		// Set settings based on full screen
		memset(&dm_screen_setteings, 0, sizeof(dm_screen_setteings));
		dm_screen_setteings.dmSize = sizeof(dm_screen_setteings);
		dm_screen_setteings.dmPelsWidth = (unsigned long)_screen_with;
		dm_screen_setteings.dmPelsHeight = (unsigned long)_screen_height;
		dm_screen_setteings.dmBitsPerPel = 32;
		dm_screen_setteings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen
		ChangeDisplaySettings(&dm_screen_setteings, CDS_FULLSCREEN);

		// Set to top let screen corner
		posX = posY = 0;
	}
	else
	{
		// Set windowed mode settings
		_screen_with = 1080;
		_screen_height = 720;

		// Postion in the midle of the screen 
		posX = (GetSystemMetrics(SM_CXSCREEN) - _screen_with) / 2.2f;
		posY = (GetSystemMetrics(SM_CXSCREEN) - _screen_height) / 20;
	}

	// Crete window
	hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		apliaction_name,
		apliaction_name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		posX,
		posY,
		_screen_with,
		_screen_height,
		NULL,
		NULL,
		hinstance,
		NULL);

	// Bring the window to front and mainfocus
	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	return;
}

void SystemClass::shutDownWindow()
{
	// Fis settings if in full screen
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remoev the window
	DestroyWindow(hwnd);
	hwnd = NULL;

	// Remove apliaction instance
	UnregisterClass(apliaction_name, hinstance);
	hinstance = NULL;

	// Relese the apliaction pointer
	aplicationHandle = NULL;

	return;
}

LRESULT CALLBACK windProc(HWND _hwnd, UINT _message, WPARAM _wparam, LPARAM _lparam)
{
	switch (_message)
	{
		// If window is being destroyed
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	// If window is being closed
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages taht will handeled
	default:
	{
		return aplicationHandle->messageHandler(_hwnd, _message, _wparam, _lparam);
	}
	}
}
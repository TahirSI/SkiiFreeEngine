// Disclude other less used apis 
#define WIN32_LEAN_AND_MEAN

#include "InputClass.h"
#include "Graphicsclass.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool initialize();
	void shutDown();
	void run();

	LRESULT CALLBACK messageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool frame();
	void intWindow(int&, int&);
	void shutDownWindow();

private:
	LPCWSTR apliaction_name;
	HINSTANCE hinstance;
	HWND hwnd;
	
	Inputclass* input;
	Graphicsclass* graphics;
};

// Global functions
static LRESULT CALLBACK windProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* aplicationHandle = 0;


#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "colourshaderclass.h"

const bool FULL_SCREEN = false;
const bool VSYINC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f; 

class Graphicsclass
{
public:
	Graphicsclass();
	Graphicsclass(const Graphicsclass&);
	~Graphicsclass();

	bool initialize(int, int, HWND);
	void shutDown();
	bool frame();

private:
	bool render();

private:
	D3dclass* dirc3D;
	Cameraclass* camera;
	Modelclass* model;
	Colourshaderclass* colour_shader;
};


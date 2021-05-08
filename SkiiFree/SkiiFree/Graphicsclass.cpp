#include "Graphicsclass.h"

Graphicsclass::Graphicsclass()
{
	dirc3D = 0;
	camera = 0;
	model = 0;
	colour_shader = 0;
}

Graphicsclass::Graphicsclass(const Graphicsclass& _other)
{
}

Graphicsclass::~Graphicsclass()
{
}

bool Graphicsclass::initialize(int _screen_with, int _screen_height, HWND _hwnd)
{
	bool result;

	// Create the Direct3D object.
	dirc3D = new D3dclass;
	if (!dirc3D)
	{
		return false;
	}

	// Int the dirc3D object.
	result = dirc3D->initialize(_screen_with, _screen_height, VSYINC_ENABLED, _hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(_hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	camera = new Cameraclass;
	if (!camera)
	{
		return false;
	}

	// Set the position of the camera.
	camera->setPosition(0, 0, -5.0f);

	// Create the model object.
	model = new Modelclass;
	if (!model)
	{
		return false;
	}

	// Int the model object.
	result = model->initialize(dirc3D->getDevice());
	if (!result)
	{
		MessageBox(_hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	colour_shader = new Colourshaderclass;
	if (!colour_shader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = colour_shader->initialize(dirc3D->getDevice(), _hwnd);
	if (!result)
	{
		MessageBox(_hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Graphicsclass::shutDown()
{
	// Release the color shader object.
	if (colour_shader)
	{
		colour_shader->shutDown();
		delete colour_shader;
		colour_shader = 0;
	}

	// Release the model object.
	if (model)
	{
		model->shutDown();
		delete model;
		model = 0;
	}

	// Release the camera object.
	if (camera)
	{
		delete camera;
		camera = 0;
	}

	// Release the Direct3D object.
	if (dirc3D)
	{
		dirc3D->shutDown();
		delete dirc3D;
		dirc3D = 0;
	}

	return;
}

bool Graphicsclass::frame()
{
	bool result;

	// Render the graphics scene.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool Graphicsclass::render()
{
	XMMATRIX worldMatrix, view_matrix, projection_matrix;
	bool result;

	// Clear the buffers to begin the scene.
	dirc3D->beginSceen(0.5f, 0.4, 1, 1);

	// Generate the view matrix based on the camera's position.
	camera->render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	dirc3D->getWorldMatrix(worldMatrix);
	camera->getViewMatrix(view_matrix);
	dirc3D->getProjectionMatrix(projection_matrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	model->render(dirc3D->getDeviceContext());

	// Render the model using the color shader.
	result = colour_shader->render(dirc3D->getDeviceContext(), model->getIndexCount(), worldMatrix, view_matrix, projection_matrix);
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	dirc3D->endSceen();

	return true;
}

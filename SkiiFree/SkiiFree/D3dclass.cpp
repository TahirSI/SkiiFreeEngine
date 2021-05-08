#include "D3dclass.h"

D3dclass::D3dclass()
{
	swap_chain = 0;
	device = 0;
	device_context = 0;
	render_target_view = 0;
	depth_stencil_buffer = 0;
	depth_stencil_state = 0;
	depth_stencil_view = 0;
	rastrize_state = 0;
}

D3dclass::D3dclass(const D3dclass&)
{
}

D3dclass::~D3dclass()
{
}

bool D3dclass::initialize(
	int _screen_with, 
	int _screen_height, 
	bool _vsync,
	HWND _hwnd, 
	bool _full_screen,
	float _screen_depth,
	float _screen_near)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adaptor_output;
	unsigned int num_modes, i, numerator = 0, denomnator = 0;
	size_t  string_length;
	DXGI_MODE_DESC* display_mode_list;
	DXGI_ADAPTER_DESC adapter_desc;
	int error;
	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	D3D_FEATURE_LEVEL feature_level;
	ID3D11Texture2D* back_buffer_ptr;
	D3D11_TEXTURE2D_DESC depth_buffer_desc;
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stecil_view_desc;
	D3D11_RASTERIZER_DESC raster_desc;
	D3D11_VIEWPORT view_port;
	float field_of_view, screen_aspect;

	// Strore vsyinc settings
	vsynic_enabled = _vsync;

	// Creet a drix graphic interface factory
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(realloc))
	{
		return false;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card)
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// Enumerate the primary adapter output (monitor)
	result = adapter->EnumOutputs(0, &adaptor_output);
	if (FAILED(result))
	{
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adaptor_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	display_mode_list = new DXGI_MODE_DESC[num_modes];
	if (!display_mode_list)
	{
		return false;
	}

	// Now fill the display mode list structures.
	result = adaptor_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, display_mode_list);
	if (FAILED(result))
	{
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i < num_modes; i++)
	{
		if (display_mode_list[i].Width == (unsigned int)_screen_with)
		{
			if (display_mode_list[i].Height == (unsigned int)_screen_height)
			{
				numerator = display_mode_list[i].RefreshRate.Numerator;
				denomnator = display_mode_list[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter(video card) description
	result = adapter->GetDesc(&adapter_desc);
	if (FAILED(result))
	{
		return false;
	}


	// Store the dedicated video card memory in megabytes.
	video_card_meemory = (int)(adapter_desc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	error = wcstombs_s(&string_length, video_card_description, 128, adapter_desc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	// Release the display mode list.
	delete[] display_mode_list;
	display_mode_list = 0;

	// Release the adapter output.
	adaptor_output->Release();
	adaptor_output = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();
	factory = 0;

	// Initialize the swap chain description.
	ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));

	// Set to a single back buffer.
	swap_chain_desc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swap_chain_desc.BufferDesc.Width = _screen_with;
	swap_chain_desc.BufferDesc.Height = _screen_height;


	// Set regular 32-bit surface for the back buffer.
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if (vsynic_enabled)
	{
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = numerator;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = denomnator;
	}
	else
	{
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = 0;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swap_chain_desc.OutputWindow = _hwnd;

	// Turn multisampling off.
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if (_full_screen)
	{
		swap_chain_desc.Windowed = false;
	}
	else
	{
		swap_chain_desc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swap_chain_desc.Flags = 0;

	// Set the feature level to DirectX 11.
	feature_level = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDeviceAndSwapChain(
		NULL, 
		D3D_DRIVER_TYPE_HARDWARE, 
		NULL, 0, &feature_level, 
		1,
		D3D11_SDK_VERSION, 
		&swap_chain_desc, 
		&swap_chain, 
		&device, 
		NULL, 
		&device_context);

	if (FAILED(result))
	{
		return false;
	}

	// Get the pointer to the back buffer.
	result = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer_ptr);
	if (FAILED(result))
	{
		return false;
	}

	// Create the render target view with the back buffer pointer.
	result = device->CreateRenderTargetView(back_buffer_ptr, NULL, &render_target_view);
	if (FAILED(result))
	{
		return false;
	}

	// Release pointer to the back buffer as we no longer need it.
	back_buffer_ptr->Release();
	back_buffer_ptr = 0;

	// Int the description of the depth buffer.
	ZeroMemory(&depth_buffer_desc, sizeof(depth_buffer_desc));

	// Set up the description of the depth buffer.
	depth_buffer_desc.Width = _screen_with;
	depth_buffer_desc.Height = _screen_height;
	depth_buffer_desc.MipLevels = 1;
	depth_buffer_desc.ArraySize = 1;
	depth_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_buffer_desc.SampleDesc.Count = 1;
	depth_buffer_desc.SampleDesc.Quality = 0;
	depth_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_buffer_desc.CPUAccessFlags = 0;
	depth_buffer_desc.MiscFlags = 0;

	// Create the texture for the depth buffer
	result = device->CreateTexture2D(&depth_buffer_desc, NULL, &depth_stencil_buffer);
	if (FAILED(result))
	{
		return false;
	}

	// Inti the description of the stencil state
	ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));

	// Set up the description of the stencil state.
	depth_stencil_desc.DepthEnable = true;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;

	depth_stencil_desc.StencilEnable = true;
	depth_stencil_desc.StencilReadMask = 0xFF;
	depth_stencil_desc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = device->CreateDepthStencilState(&depth_stencil_desc, &depth_stencil_state);
	if (FAILED(result))
	{
		return false;
	}

	// Set the depth stencil state.
	device_context->OMSetDepthStencilState(depth_stencil_state, 1);

	// Int the depth stencil view.
	ZeroMemory(&depth_stecil_view_desc, sizeof(depth_stecil_view_desc));

	// Set up the depth stencil view description.
	depth_stecil_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_stecil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_stecil_view_desc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = device->CreateDepthStencilView(depth_stencil_buffer, &depth_stecil_view_desc, &depth_stencil_view);
	if (FAILED(result))
	{
		return false;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline
	device_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	// Setup the raster description which will determine how and what polygons will be drawn.
	raster_desc.AntialiasedLineEnable = false;
	raster_desc.CullMode = D3D11_CULL_BACK;
	raster_desc.DepthBias = 0;
	raster_desc.DepthBiasClamp = 0.0f;
	raster_desc.DepthClipEnable = true;
	raster_desc.FillMode = D3D11_FILL_SOLID;
	raster_desc.FrontCounterClockwise = false;
	raster_desc.MultisampleEnable = false;
	raster_desc.ScissorEnable = false;
	raster_desc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = device->CreateRasterizerState(&raster_desc, &rastrize_state);
	if (FAILED(result))
	{
		return false;
	}

	// Now set the rasterizer state.
	device_context->RSSetState(rastrize_state);

	// Setup the viewport for rendering.
	view_port.Width = (float)_screen_with;
	view_port.Height = (float)_screen_height;
	view_port.MinDepth = 0.0f;
	view_port.MaxDepth = 1.0f;
	view_port.TopLeftX = 0.0f;
	view_port.TopLeftY = 0.0f;

	// Create the viewport.
	device_context->RSSetViewports(1, &view_port);

	// Setup the projection matrix.
	field_of_view = 3.141592654f / 4.0f;
	screen_aspect = (float)_screen_with / (float)_screen_height;

	// Create the projection matrix for 3D rendering.
	projection_matrix =  XMMatrixPerspectiveFovLH(field_of_view, screen_aspect, screen_aspect, _screen_depth);

	// Int world matrix to the identity matrix.
	world_matrix = XMMatrixIdentity();

	// Create an orthographic projection matrix for 2D rendering.
	ortho_matrix = XMMatrixOrthographicLH((float)_screen_with, (float)_screen_height, _screen_near, _screen_depth);

	return true;
}

void D3dclass::shutDown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (swap_chain)
	{
		swap_chain->SetFullscreenState(false, NULL);
	}

	if (rastrize_state)
	{
		rastrize_state->Release();
		rastrize_state = 0;
	}

	if (depth_stencil_view)
	{
		depth_stencil_view->Release();
		depth_stencil_view = 0;
	}

	if (depth_stencil_state)
	{
		depth_stencil_state->Release();
		depth_stencil_state = 0;
	}

	if (depth_stencil_buffer)
	{
		depth_stencil_buffer->Release();
		depth_stencil_buffer = 0;
	}

	if (render_target_view)
	{
		render_target_view->Release();
		render_target_view = 0;
	}

	if (device_context)
	{
		device_context->Release();
		device_context = 0;
	}

	if (device)
	{
		device->Release();
		device = 0;
	}

	if (swap_chain)
	{
		swap_chain->Release();
		swap_chain = 0;
	}

	return;
}

void D3dclass::beginSceen(float _red, float _green, float _blue, float _salpha)
{
	float colour[4];

	// Set the colour
	colour[0] = _red;
	colour[1] = _green;
	colour[2] = _blue;
	colour[3] = _salpha;

	// Clear the back buffer
	device_context->ClearRenderTargetView(render_target_view, colour);

	// Clear the depth buffer
	device_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void D3dclass::endSceen()
{
	// Present the back buffer to the screen since rendering is complete.
	if (vsynic_enabled)
	{
		// Lock to screen refresh rate.
		swap_chain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		swap_chain->Present(0, 0);
	}

	return;
}

ID3D11Device* D3dclass::getDevice()
{
	return device;
}

ID3D11DeviceContext* D3dclass::getDeviceContext()
{
	return device_context;
}

void D3dclass::getProjectionMatrix(XMMATRIX& _projectionMatrix)
{
	_projectionMatrix = projection_matrix;
	return;
}

void D3dclass::getWorldMatrix(XMMATRIX& _worldMatrix)
{
	_worldMatrix = world_matrix;
	return;
}

void D3dclass::getOrthoMatrix(XMMATRIX& _orthoMatrix)
{
	_orthoMatrix = ortho_matrix;
	return;
}

void D3dclass::getVideoCardInfo(char* _cardName, int& _memory)
{
	strcpy_s(_cardName, 128, video_card_description);
	_memory = video_card_meemory;
	return;
}

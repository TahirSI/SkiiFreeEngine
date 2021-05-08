#include "Colourshaderclass.h"

Colourshaderclass::Colourshaderclass()
{
	vertex_shader = 0;
	pixel_shader = 0;
	layout = 0;
	matrix_buffer = 0;
}

Colourshaderclass::Colourshaderclass(const Colourshaderclass& _other)
{
}

Colourshaderclass::~Colourshaderclass()
{
}

bool Colourshaderclass::initialize(ID3D11Device* _device, HWND _hwnd)
{
	bool result;

	// Inti the vertex and pixel shaders
	result = intiShader(_device, _hwnd, L"../SkiiFree/Color.vs", L"../SkiiFree/Color.ps");
	if (!result)
	{
		return false;
	}

	return true;
}

void Colourshaderclass::shutDown()
{
	// Shutdown the shader
	shutDownShader();

	return;
}

bool Colourshaderclass::render(
	ID3D11DeviceContext* _device_context, 
	int _index_count, 
	XMMATRIX _world_matrix, 
	XMMATRIX _view_matrix,
	XMMATRIX _projection_matrix)
{
	bool result;

	result = setShaderParameters(_device_context, _world_matrix, _view_matrix, _projection_matrix);
	if (!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	renderShader(_device_context, _index_count);

	return true;
}

bool Colourshaderclass::intiShader(ID3D11Device* _device, HWND _hwnd, LPCWSTR _vs_file_name, LPCWSTR _ps_file_name)
{
	HRESULT result;
	ID3D10Blob* error_message = 0;
	ID3D10Blob* vertix_shader_buffer = 0;
	ID3D10Blob* pixel_shader_buffer = 0;
	D3D11_INPUT_ELEMENT_DESC polygon_Layout[2];
	unsigned int num_of_elements;
	D3D11_BUFFER_DESC matrix_buffer_desc;

	// Compile the vertex shader code.
	result = D3DCompileFromFile(
		_vs_file_name, 
		NULL, 
		NULL, 
		"ColorVertexShader", 
		"vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 
		0,
		&vertix_shader_buffer, &error_message);

	if (FAILED(result))
	{
		// If the shader failed
		if (error_message)
		{
			outputShaderErrorMessage(error_message, _hwnd, _vs_file_name);
		}
		// If there was nothing in the error message
		else
		{
			MessageBox(_hwnd, _vs_file_name, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Compile the pixel shader code
	result = D3DCompileFromFile(
		_ps_file_name, 
		NULL, 
		NULL, 
		"ColorPixelShader", 
		"ps_5_0", 
		D3D10_SHADER_ENABLE_STRICTNESS, 
		0,
		&pixel_shader_buffer, 
		&error_message);

	if (FAILED(result))
	{
		// If the shader failed to compile
		if (error_message)
		{
			outputShaderErrorMessage(error_message, _hwnd, _ps_file_name);
		}
		// If there was nothing in the error message
		else
		{
			MessageBox(_hwnd, _ps_file_name, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Create the vertex shader from the buffer.
	result = _device->CreateVertexShader(vertix_shader_buffer->GetBufferPointer(), vertix_shader_buffer->GetBufferSize(), NULL, &vertex_shader);
	if (FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = _device->CreatePixelShader(pixel_shader_buffer->GetBufferPointer(), pixel_shader_buffer->GetBufferSize(), NULL, &pixel_shader);
	if (FAILED(result))
	{
		return false;
	}

	// Create the vertex input layout description
	polygon_Layout[0].SemanticName = "POSITION";
	polygon_Layout[0].SemanticIndex = 0;
	polygon_Layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygon_Layout[0].InputSlot = 0;
	polygon_Layout[0].AlignedByteOffset = 0;
	polygon_Layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygon_Layout[0].InstanceDataStepRate = 0;

	polygon_Layout[1].SemanticName = "COLOR";
	polygon_Layout[1].SemanticIndex = 0;
	polygon_Layout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygon_Layout[1].InputSlot = 0;
	polygon_Layout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygon_Layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygon_Layout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	num_of_elements = sizeof(polygon_Layout) / sizeof(polygon_Layout[0]);

	// Create the vertex input layout.
	result = _device->CreateInputLayout(
		polygon_Layout, num_of_elements, 
		vertix_shader_buffer->GetBufferPointer(),
		vertix_shader_buffer->GetBufferSize(), 
		&layout);

	if (FAILED(result))
	{
		return false;
	}

	// Realse the vertix shader bufer
	vertix_shader_buffer->Release();
	vertix_shader_buffer = 0;

	// Realse the pixel shader buffer
	pixel_shader_buffer->Release();
	pixel_shader_buffer = 0;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrix_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	matrix_buffer_desc.ByteWidth = sizeof(matrixBufferType);
	matrix_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrix_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrix_buffer_desc.MiscFlags = 0;
	matrix_buffer_desc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = _device->CreateBuffer(&matrix_buffer_desc, NULL, &matrix_buffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void Colourshaderclass::shutDownShader()
{
	// Release the matrix constant buffer.
	if (matrix_buffer)
	{
		matrix_buffer->Release();
		matrix_buffer = 0;
	}

	// Release the layout.
	if (layout)
	{
		layout->Release();
		layout = 0;
	}

	// Release the pixel shader.
	if (pixel_shader)
	{
		pixel_shader->Release();
		pixel_shader = 0;
	}

	// Release the vertex shader.
	if (vertex_shader)
	{
		vertex_shader->Release();
		vertex_shader = 0;
	}

	return;
}

void Colourshaderclass::outputShaderErrorMessage(ID3D10Blob* _error_message, HWND _hwnd, LPCWSTR _shader_file_name)
{
	char* compier_errors;
	size_t buffer_size, i;
	ofstream fout;

	// Get a pointer to the error message text buffer.
	compier_errors = (char*)(_error_message->GetBufferPointer());

	// Get the length of the message.
	buffer_size = _error_message->GetBufferSize();

	// Open a file to write the error message
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i < buffer_size; i++)
	{
		fout << compier_errors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	_error_message->Release();
	_error_message = 0;

	// Pop a message up on the screen
	MessageBox(_hwnd, L"Error compiling shader.  Check shader-error.txt for message.", _shader_file_name, MB_OK);

	return;
}

bool Colourshaderclass::setShaderParameters(
	ID3D11DeviceContext* _device_context, 
	XMMATRIX world_matrix, 
	XMMATRIX _view_matrix,
	XMMATRIX _projection_matrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mapped_resource;
	matrixBufferType* data_ptr;
	unsigned int buffer_number = 0;

	// Transpose the matrices to prepare them for the shader.
	world_matrix = XMMatrixTranspose(world_matrix);
	_view_matrix = XMMatrixTranspose(_view_matrix);
	_projection_matrix = XMMatrixTranspose(_projection_matrix);

	// Lock the constant buffer so it can be written to.
	result = _device_context->Map(matrix_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	data_ptr = (matrixBufferType*)mapped_resource.pData;

	// Copy the matrices into the constant buffer.
	data_ptr->world = world_matrix;
	data_ptr->view = _view_matrix;
	data_ptr->projection = _projection_matrix;

	// Unlock the constant buffer.
	_device_context->Unmap(matrix_buffer, 0);


	// Finanly set the constant buffer in the vertex shader with the updated values.
	_device_context->VSSetConstantBuffers(buffer_number, 1, &matrix_buffer);

	return true;
}

void Colourshaderclass::renderShader(ID3D11DeviceContext* _device_context, int _index_count)
{
	// Sets the vertex input layout.
	_device_context->IASetInputLayout(layout);

	// Set the vertex and pixel shaders
	_device_context->VSSetShader(vertex_shader, NULL, 0);
	_device_context->PSSetShader(pixel_shader, NULL, 0);

	// Render the triangle.
	_device_context->DrawIndexed(_index_count, 0, 0);

	return;
}

#include "Modelclass.h"

Modelclass::Modelclass()
{
	vertex_buffer = 0;
	index_buffer = 0;
}

Modelclass::Modelclass(const Modelclass&)
{
}

Modelclass::~Modelclass()
{
}

bool Modelclass::initialize(ID3D11Device* _device)
{
	bool result;

	// Int the vertex and index buffers.
	result = initBuffers(_device);
	if (!result)
	{
		return false;
	}

	return true;
}

void Modelclass::shutDown()
{
	// Shutdown the vertex and index bufferes
	shutDownBuffer();

	return;
}

void Modelclass::render(ID3D11DeviceContext*_device_contex)
{
	renderBuffers(_device_contex);

	return;
}

int Modelclass::getIndexCount()
{
	return index_count;
}

bool Modelclass::initBuffers(ID3D11Device* _device)
{
	vertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertex_buffer_desc, index_buffer_desc;
	D3D11_SUBRESOURCE_DATA vertex_data, index_data;
	HRESULT result;

	// Set the number of vertices
	vertex_cout = 3;

	// Set the number of indices
	index_count = vertex_cout;

	// Create the vertex array.
	vertices = new vertexType[vertex_cout];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[index_count];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array with data.
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.

	// Set up the description of the static vertex buffer.
	vertex_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	vertex_buffer_desc.ByteWidth = sizeof(vertexType) * vertex_cout;
	vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertex_buffer_desc.CPUAccessFlags = 0;
	vertex_buffer_desc.MiscFlags = 0;
	vertex_buffer_desc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertex_data.pSysMem = vertices;
	vertex_data.SysMemPitch = 0;
	vertex_data.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = _device->CreateBuffer(&vertex_buffer_desc, &vertex_data, &vertex_buffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	index_buffer_desc.ByteWidth = sizeof(unsigned long) * index_count;
	index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	index_buffer_desc.CPUAccessFlags = 0;
	index_buffer_desc.MiscFlags = 0;
	index_buffer_desc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	index_data.pSysMem = indices;
	index_data.SysMemPitch = 0;
	index_data.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = _device->CreateBuffer(&index_buffer_desc, &index_data, &index_buffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex and index buffers have been created and loaded
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void Modelclass::shutDownBuffer()
{
	// Release the index buffer.
	if (index_buffer)
	{
		index_buffer->Release();
		index_buffer = 0;
	}

	// Release the vertex buffer.
	if (vertex_buffer)
	{
		vertex_buffer->Release();
		vertex_buffer = 0;
	}

	return;
}

void Modelclass::renderBuffers(ID3D11DeviceContext* _device_context)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(vertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	_device_context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	_device_context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

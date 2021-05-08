#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>

using namespace DirectX;
using namespace std;

class Colourshaderclass
{
private:

	struct matrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	Colourshaderclass();
	Colourshaderclass(const Colourshaderclass&);
	~Colourshaderclass();

	bool initialize(ID3D11Device*, HWND);
	void shutDown();
	bool render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX);

private:
	bool intiShader(ID3D11Device*, HWND, LPCWSTR, LPCWSTR);
	void shutDownShader();
	void outputShaderErrorMessage(ID3D10Blob*, HWND, LPCWSTR);

	bool setShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);
	void renderShader(ID3D11DeviceContext*, int);

	ID3D11VertexShader* vertex_shader;
	ID3D11PixelShader* pixel_shader;
	ID3D11InputLayout* layout;
	ID3D11Buffer* matrix_buffer;
};


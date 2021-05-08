#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class D3dclass
{
public:
	D3dclass();
	D3dclass(const D3dclass&);
	~D3dclass();

	bool initialize(int, int, bool, HWND, bool, float, float);
	void shutDown();

	void beginSceen(float, float, float, float);
	void endSceen();

	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();

	void getProjectionMatrix(XMMATRIX&);
	void getWorldMatrix(XMMATRIX&);
	void getOrthoMatrix(XMMATRIX&);

	void getVideoCardInfo(char*, int&);

private:
	bool vsynic_enabled;
	int video_card_meemory;
	char video_card_description[128];
	IDXGISwapChain* swap_chain;
	ID3D11Device* device;
	ID3D11DeviceContext* device_context;
	ID3D11RenderTargetView* render_target_view;
	ID3D11Texture2D* depth_stencil_buffer;
	ID3D11DepthStencilState* depth_stencil_state;
	ID3D11DepthStencilView* depth_stencil_view;
	ID3D11RasterizerState* rastrize_state;
	XMMATRIX projection_matrix;
	XMMATRIX world_matrix;
	XMMATRIX ortho_matrix;
};


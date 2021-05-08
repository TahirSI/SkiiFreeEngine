#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class Modelclass
{
private:
	struct vertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

public:
	Modelclass();
	Modelclass(const Modelclass&);
	~Modelclass();

	bool initialize(ID3D11Device*);
	void shutDown();
	void render(ID3D11DeviceContext*);

	int getIndexCount();

private:
	bool initBuffers(ID3D11Device*);
	void shutDownBuffer();
	void renderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer* vertex_buffer, *index_buffer;
	int vertex_cout, index_count ;
};


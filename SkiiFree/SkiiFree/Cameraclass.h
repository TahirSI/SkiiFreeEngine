#include <directxmath.h>
using namespace DirectX;

class Cameraclass
{
public:
	Cameraclass();
	Cameraclass(const Cameraclass&);
	~Cameraclass();

	void setPosition(float, float, float);
	void setRotation(float, float, float);

	XMFLOAT3 getPosition();
	XMFLOAT3 getRotation();

	void render();
	void getViewMatrix(XMMATRIX&);

private:
	float positionX, positionY, positionZ;
	float rotationX, rotationY, rotationZ;
	XMMATRIX view_matrix;
};


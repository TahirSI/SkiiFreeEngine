#include "Cameraclass.h"

Cameraclass::Cameraclass()
{
	// Postions
	positionX = 0.0f;
	positionY = 0.0f;
	positionZ = 0.0f;

	// Rotations
	rotationX = 0.0f;
	rotationY = 0.0f;
	rotationZ = 0.0f;
}

Cameraclass::Cameraclass(const Cameraclass& _other)
{
}

Cameraclass::~Cameraclass()
{
}

void Cameraclass::setPosition(float _posX, float _posY, float _posZ)
{
	positionX = _posX;
	positionY = _posY;
	positionZ = _posZ;
	return;
}

void Cameraclass::setRotation(float _rotX, float _rotY, float _rotZ)
{
	rotationX = _rotX;
	rotationY = _rotY;
	rotationZ = _rotZ;
	return;
}

XMFLOAT3 Cameraclass::getPosition()
{
	return XMFLOAT3(positionX, positionY, positionZ);
}

XMFLOAT3 Cameraclass::getRotation()
{
	return XMFLOAT3(rotationX, rotationY, rotationZ);
}

void Cameraclass::render()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR up_vector, position_vector, lookAt_vector;
	float yaw, pitch, roll;
	XMMATRIX rotation_matrix;

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Load it into a XMVECTOR structure.
	up_vector = XMLoadFloat3(&up);

	// Setup the position of the camera in the world.
	position.x = positionX;
	position.y = positionY;
	position.z = positionZ;

	// Load it into a XMVECTOR structure.
	position_vector = XMLoadFloat3(&position);

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Load it into a XMVECTOR structure.
	lookAt_vector = XMLoadFloat3(&lookAt);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = rotationX * 0.0174532925f;
	yaw = rotationY * 0.0174532925f;
	roll = rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotation_matrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAt_vector = XMVector3TransformCoord(lookAt_vector, rotation_matrix);
	up_vector = XMVector3TransformCoord(up_vector, rotation_matrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt_vector = XMVectorAdd(position_vector, lookAt_vector);

	// Finally create the view matrix from the three updated vectors.
	view_matrix = XMMatrixLookAtLH(position_vector, lookAt_vector, up_vector);

	return;
}

void Cameraclass::getViewMatrix(XMMATRIX& _view_matrix)
{
	_view_matrix = view_matrix;
	return;
}

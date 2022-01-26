#include "Camera.h"

/*
	Constructors needed to create an instance of a camera to view the model
*/

Camera::Camera()
{

	_xRotation = 0.0f;
	_yRotation = 0.0f;
	_zRotation = 0.0f;

	_viewingPosition = Vertex(0, 0, -50, 1);

	CameraMatrix = Matrix{ 1, 0, 0, 0,
							0, cos(_xRotation), sin(_xRotation), 0,
							0, -sin(_xRotation), cos(_xRotation), 0,
							0, 0, 0, 1 } *

				   Matrix{ cos(_yRotation), 0, -sin(_yRotation), 0,
							0, 1, 0, 0,
							sin(_yRotation), 0, cos(_yRotation), 0,
							0, 0, 0, 1 } *

				   Matrix{ cos(_zRotation), sin(_zRotation), 0, 0,
							-sin(_zRotation), cos(_zRotation), 0, 0,
							0, 0, 1, 0,
							0, 0, 0, 1 } *

				   Matrix{1, 0, 0, 0,
						  0, 1, 0, 0,
						  0, 0, 1, 0,
						  0, 0, 0, 1 };

}

Camera::Camera(float xRotation, float yRotation, float zRotation, const Vertex& position)
{

	_xRotation = xRotation;
	_yRotation = yRotation;
	_zRotation = zRotation;

	_viewingPosition = position;

	_xTranslation = _viewingPosition.GetX();
	_yTranslation = _viewingPosition.GetY();
	_zTranslation = _viewingPosition.GetZ();

	CameraMatrix = Matrix{ 1, 0, 0, 0,
							0, cos(_xRotation), sin(_xRotation), 0,
							0, -sin(_xRotation), cos(_xRotation), 0,
							0, 0, 0, 1 } *

		Matrix{ cos(_yRotation), 0, -sin(_yRotation), 0,
				 0, 1, 0, 0,
				 sin(_yRotation), 0, cos(_yRotation), 0,
				 0, 0, 0, 1 } *

		Matrix{ cos(_zRotation), sin(_zRotation), 0, 0,
				 -sin(_zRotation), cos(_zRotation), 0, 0,
				 0, 0, 1, 0,
				 0, 0, 0, 1 } *

		Matrix{ 1, 0, 0, -_xTranslation,
			   0, 1, 0, -_yTranslation,
			   0, 0, 1, -_zTranslation,
			   0, 0, 0, 1 };

}

Camera::~Camera()
{
}

/*
	Accesses / mutates the camera position and rotation values
	that the camera needs to be created and manipulated
*/

float Camera::GetXRotation() const
{
	return _xRotation;
}

void Camera::SetXRotation(const float xRotation)
{
	_xRotation = xRotation;
}

float Camera::GetYRotation() const
{
	return _yRotation;
}

void Camera::SetYRotation(const float yRotation)
{
	_yRotation = yRotation;
}

float Camera::GetZRotation() const
{
	return _zRotation;
}

void Camera::SetZRotation(const float zRotation)
{
	_zRotation = zRotation;
}

Vertex Camera::GetCameraPosition() const
{
	return _viewingPosition;
}

/*
	Carries out the calculation needed to take rotations and
	translations of the camera matrix into consideration before
	the user views anything
*/

Matrix Camera::CreateViewingMatrix()
{
	return CameraMatrix;
}

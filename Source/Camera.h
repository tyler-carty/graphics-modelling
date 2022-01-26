#pragma once
#include "Vertex.h"
#include "Matrix.h"
#include <cmath>

class Camera
{
public:

	/*
	Definition of the constructors needed to create an instance
	of a camera to view the model
	*/

	Camera();
	Camera(float xRotation, float yRotation, float zRotation, const Vertex& position);
	~Camera();

	/*
	Accesses / mutates the camera position and rotation values 
	that the camera needs to be created and manipulated
	*/

	float GetXRotation() const;
	void SetXRotation(const float xRotation);

	float GetYRotation() const;
	void SetYRotation(const float yRotation);

	float GetZRotation() const;
	void SetZRotation(const float zRotation);

	Vertex GetCameraPosition() const;

	/*
	Carries out the calculation needed to take rotations and 
	translations of the camera matrix into consideration before
	the user views anything
	*/

	Matrix CreateViewingMatrix();

private:

	/*
	Members to hold the values needed for various camera transformations/
	matrix calculations
	*/
	
	float _xRotation;
	float _yRotation;
	float _zRotation;

	float _xTranslation;
	float _yTranslation;
	float _zTranslation;

	Matrix CameraMatrix;

	Vertex _viewingPosition;

};

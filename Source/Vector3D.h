#pragma once
//#include "Vertex.h"
#include <cmath>

class Vector3D
{
public:

	/*
	Definition of the constructors needed to create an instance
	of a 3D vector
	*/

	Vector3D();
	Vector3D(float x, float y, float z);
	~Vector3D();

	/*
	Accesses / mutates the XYZ values that the vector is made of
	*/

	float GetX() const;
	void SetX(const float x);
	float GetY() const;
	void SetY(const float y);
	float GetZ() const;
	void SetZ(const float z);

	/*
	Methods to find the Dot/Cross product of two vectors, 
	normalise a vector or calculate the magnitude of a vector
	*/

	static float CreateDotProduct(const Vector3D& normalVector, const Vector3D& eyeVector);
	static Vector3D CreateCrossProduct(const Vector3D& vectorNormal , const Vector3D& vectorEye);
	static Vector3D NormaliseVector(const Vector3D& lightingVector);
	static float CalculateMagnitude(const Vector3D& vector);

	/*
	Operators in order to add and divide vectors with each other
	*/

	const Vector3D operator+ (const Vector3D& rhs) const;
	const Vector3D operator/ (const int& rhs) const;

private:
	
	/*
	Members to hold the XYZ values of the vector
	*/

	float _x;
	float _y;
	float _z;

};
#include "Vector3D.h"

/*
	Constructors needed to create an instance
	of a 3D vector
*/
Vector3D::Vector3D()
{
	_x = 0.0f;
	_y = 0.0f;
	_z = 0.0f;
}

Vector3D::Vector3D(float x, float y, float z)
{
	_x = x;
	_y = y;
	_z = z;

}

Vector3D::~Vector3D()
{
}

/*
	Accesses / mutates the XYZ values that the vector is made of
*/
float Vector3D::GetX() const
{
	return _x;
}

void Vector3D::SetX(const float x)
{
	_x = x;
}

float Vector3D::GetY() const
{
	return _y;
}

void Vector3D::SetY(const float y)
{
	_y = y;
}

float Vector3D::GetZ() const
{
	return _z;
}

void Vector3D::SetZ(const float z)
{
	_z = z;
}

/*
	Methods to find the Dot/Cross product of two vectors,
	normalise a vector or calculate the magnitude of a vector
*/
float Vector3D::CreateDotProduct(const Vector3D& vectorNormal, const Vector3D& vectorEye)
{
	return(vectorEye.GetX() * vectorNormal.GetX() + vectorEye.GetY() * vectorNormal.GetY() + vectorEye.GetZ() * vectorNormal.GetZ());
}

Vector3D Vector3D::CreateCrossProduct(const Vector3D& vectorA, const Vector3D& vectorB)
{

	float normalVertexAX = vectorA.GetX();
	float normalVertexAY = vectorA.GetY();
	float normalVertexAZ = vectorA.GetZ();

	float normalVertexBX = vectorB.GetX();
	float normalVertexBY = vectorB.GetY();
	float normalVertexBZ = vectorB.GetZ();

	return(Vector3D(normalVertexAY * normalVertexBZ - 
		normalVertexAZ * normalVertexBY, normalVertexAZ * normalVertexBX - normalVertexAX * 
		normalVertexBZ, normalVertexAX * normalVertexBY - normalVertexAY * normalVertexBX));
}

Vector3D Vector3D::NormaliseVector(const Vector3D& lightingVector)
{

	float total = sqrt(lightingVector.GetX()* lightingVector.GetX() + lightingVector.GetY()* lightingVector.GetY() + lightingVector.GetZ()* lightingVector.GetZ());

	float vectorX = lightingVector.GetX() / total;
	float vectorY = lightingVector.GetY() / total;
	float vectorZ = lightingVector.GetZ() / total;

	return Vector3D(vectorX, vectorY, vectorZ);
}

float Vector3D::CalculateMagnitude(const Vector3D& vector)
{
	return(sqrt(vector.GetX() * vector.GetX() + vector.GetY() * vector.GetY() + vector.GetZ() * vector.GetZ()));
}

/*
	Operators in order to add and divide vectors with each other
*/
const Vector3D Vector3D::operator+(const Vector3D& rhs) const
{
	return Vector3D(_x + rhs.GetX(), _y + rhs.GetY(), _z + rhs.GetZ());;
}

const Vector3D Vector3D::operator/(const int& rhs) const
{
	return Vector3D(_x / rhs, _y / rhs, _z / rhs);
}

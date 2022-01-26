#include "Vertex.h"

/*
	Constructors needed to create an instance of a vertex
*/
Vertex::Vertex()
{
	_x = 0.0f;
	_y = 0.0f;
	_z = 0.0f;
	_w = 0.0f;

	_vertexNormal = Vector3D(0, 0, 0);
	_vertexRGB = RGB(0, 0, 0);
	_contributeCount = 0;

	_uvCoord = UVCoord(0.0f, 0.0f);

	_preTransformZ = 0;

	_uOverZ = 0;
	_vOverZ = 0;
	_zReciprocal = 0;
}

Vertex::Vertex(float x, float y, float z, float w)
{
	_x = x;
	_y = y;
	_z = z;
	_w = w;

	_vertexNormal = Vector3D(0, 0, 0);
	_vertexRGB = RGB(0, 0, 0);
	_contributeCount = 0;

	_uvCoord = UVCoord(0.0f, 0.0f);

	_preTransformZ = 0;

	_uOverZ = 0;
	_vOverZ = 0;
	_zReciprocal = 0;
}

Vertex::Vertex(const Vertex& other)
{
	_x = other.GetX();
	_y = other.GetY();
	_z = other.GetZ();
	_w = other.GetW();

	_vertexNormal = other.GetVertexNormal();
	_vertexRGB = other.GetVertexRGB();
	_contributeCount = other.GetContributeCount();

	_uvCoord = other.GetUVCoord();

	_preTransformZ = other.GetPreTranZ();

	_uOverZ = other.GetUOZ();
	_vOverZ = other.GetVOZ();
	_zReciprocal = other.GetZR();
}

/*
	Method to dehomogenize the coordinates of a vertex
*/
void Vertex::Dehomogenized()
{
	_x = _x / _w;
	_y = _y / _w;
	_z = _z / _w;
	_w = 1;
}

/*
	Accessors/Mutators to get and set all XYZW values of a given vertex
*/
float Vertex::GetX() const
{
	return _x;
}

void Vertex::SetX(const float x)
{
	_x = x;
}

float Vertex::GetY() const
{
	return _y;
}

void Vertex::SetY(const float y)
{
	_y = y;
}

float Vertex::GetZ() const
{
	return _z;
}

void Vertex::SetZ(const float z)
{
	_z = z;
}

float Vertex::GetW() const
{
	return _w;
}

void Vertex::SetW(const float w)
{
	_w = w;
}

/*
	Accessors/Mutators to get and set the normal, colour and contribution counter of a given vertex
*/
Vector3D Vertex::GetVertexNormal() const
{
	return _vertexNormal;
}

void Vertex::SetVertexNormal(const Vector3D& vertexNormal)
{
	_vertexNormal = vertexNormal;
}

int Vertex::GetVertexRGB() const
{
	return _vertexRGB;
}

void Vertex::SetVertexRGB(const int& vertexRGB)
{
	_vertexRGB = vertexRGB;
}

int Vertex::GetContributeCount() const
{
	return _contributeCount;
}

void Vertex::SetContributeCount(const int& contributeCount)
{
	_contributeCount = contributeCount;
}

/*
	Accessor/Mutator to get and set the UV coordinate of a given vertex
*/
UVCoord Vertex::GetUVCoord() const
{
	return _uvCoord;
}

void Vertex::SetUVCoord(const UVCoord& uvCoord)
{
	_uvCoord = uvCoord;
}

/*
	Accessor to get the XY coordinate of a given vertex as an integer
*/
int Vertex::GetIntX() const
{
	return (int)_x;
}

int Vertex::GetIntY() const
{
	return (int)_y;
}

/*
	Accessor/Mutator to get the pre-transformation z coordinate of a given vertex
*/
float Vertex::GetPreTranZ() const
{
	return _preTransformZ;
}

void Vertex::SetPreTranZ(const float& preTranZ)
{
	_preTransformZ = preTranZ;
}

/*
	Accessors/Mutators to get the calculation results necessary for the texture mapping of a given vertex
*/
float Vertex::GetUOZ() const
{
	return _uOverZ;
}

void Vertex::SetUOZ(const float& uOverZ)
{
	_uOverZ = uOverZ;
}

float Vertex::GetVOZ() const
{
	return _vOverZ;
}

void Vertex::SetVOZ(const float& vOverZ)
{
	_vOverZ = vOverZ;
}

float Vertex::GetZR() const
{
	return _zReciprocal;
}

void Vertex::SetZR(const float& zReciprocal)
{
	_zReciprocal = zReciprocal;
}

/*
	Assignment operator followed by a check operator to see if a vertex is equal to another
*/
Vertex& Vertex::operator=(const Vertex& rhs)
{
	// Only do the assignment if we are not assigning
	// to ourselves
	if (this != &rhs)
	{
		_x = rhs.GetX();
		_y = rhs.GetY();
		_z = rhs.GetZ();
		_w = rhs.GetW();

		_vertexNormal = rhs.GetVertexNormal();
		_vertexRGB = rhs.GetVertexRGB();
		_contributeCount = rhs.GetContributeCount();

		_uvCoord = rhs.GetUVCoord();

		_preTransformZ = rhs.GetPreTranZ();

		_uOverZ = rhs.GetUOZ();
		_vOverZ = rhs.GetVOZ();
		_zReciprocal = rhs.GetZR();
	}
	return *this;
}

// The const at the end of the declaraion for '==" indicates that this operation does not change
// any of the member variables in this class.

bool Vertex::operator==(const Vertex& rhs) const
{
	return (_x == rhs.GetX() && _y == rhs.GetY() && _z == rhs.GetZ() && _w == rhs.GetW());
}

// You can see three different uses of 'const' here:
//
// The first const indicates that the method changes the return value, but it is not moved in memory
// The second const indicates that the parameter is passed by reference, but it is not modified
// The third const indicates that the operator does not change any of the memory variables in the class.

const Vertex Vertex::operator+(const Vertex& rhs) const
{
	return Vertex(_x + rhs.GetX(), _y + rhs.GetY(), _z + rhs.GetZ(), _w + rhs.GetW());
}

const Vector3D Vertex::operator-(const Vertex& rhs) const
{
	return Vector3D(_x - rhs.GetX(), _y - rhs.GetY(), _z - rhs.GetZ());
}









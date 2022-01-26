#include "Polygon3D.h"

/*
	Definition of the constructors needed to create an instance
	of a polygon on the model
*/
Polygon3D::Polygon3D()
{
	_indices[0] = 0;
	_indices[1] = 0;
	_indices[2] = 0;

	_toBeCulled = false;
	_averageZ = 0.0f;
	_polygonNormal = Vector3D(0, 0, 0);

	_uvIndices[0] = 0;
	_uvIndices[1] = 0;
	_uvIndices[2] = 0;

	_RGBValue = RGB(0, 0, 0);
}


//assign each member index a value
Polygon3D::Polygon3D(int index0, int index1, int index2, int uvIndex0, int uvIndex1, int uvIndex2)
{
	_indices[0] = index0;
	_indices[1] = index1;
	_indices[2] = index2;

	_uvIndices[0] = uvIndex0;
	_uvIndices[1] = uvIndex1;
	_uvIndices[2] = uvIndex2;

	_toBeCulled = false;
	_averageZ = 0.0f;
	_polygonNormal = Vector3D(0, 0, 0);

	_RGBValue = RGB(0, 0, 0);
}


Polygon3D::~Polygon3D()
{

}

//copy constructor of p
Polygon3D::Polygon3D(const Polygon3D& p)
{
	_indices[0] = p._indices[0];
	_indices[1] = p._indices[1];
	_indices[2] = p._indices[2];

	_uvIndices[0] = p._uvIndices[0];
	_uvIndices[1] = p._uvIndices[1];
	_uvIndices[2] = p._uvIndices[2];

	_polygonNormal = p._polygonNormal;

	_toBeCulled = p._toBeCulled;
	_averageZ = p._averageZ;
	_RGBValue = p._RGBValue;
}

/*
	Accessors to return polygon index/UV coordinate index of specified vertex
*/
int Polygon3D::GetIndex(int currentIndex) const
{
	return _indices[currentIndex];
}

int Polygon3D::GetUVIndex(int currentIndex) const
{
	return _uvIndices[currentIndex];
}

/*
	Accesses / mutates the state of the current polygon
	culling boolean and average Z depth value

	Accesses / mutates the RGB value of the polygon
	as well as its normal vector
*/

bool Polygon3D::GetCullState() const
{
	return _toBeCulled;
}

void Polygon3D::SetCullState(const bool cullState)
{
	_toBeCulled = cullState;
}

float Polygon3D::GetAverageZ() const
{
	return _averageZ;
}

void Polygon3D::SetAverageZ(const float averagePolygonZ)
{
	_averageZ = averagePolygonZ;
}

COLORREF Polygon3D::GetRGBValue() const
{
	return _RGBValue;
}

void Polygon3D::SetRGBValue(const COLORREF& RGBValue)
{
	_RGBValue = RGBValue;
}

Vector3D Polygon3D::GetPolygonNormal() const
{
	return _polygonNormal;
}

void Polygon3D::SetPolygonNormal(const Vector3D& vectorNormal)
{
	_polygonNormal = vectorNormal;
}

//copies the rhs of the argument
Polygon3D& Polygon3D::operator=(const Polygon3D& rhs)
{
	if (this != &rhs) 
	{
		_indices[0] = rhs._indices[0];
		_indices[1] = rhs._indices[1];
		_indices[2] = rhs._indices[2];

		_uvIndices[0] = rhs.GetUVIndex(0);
		_uvIndices[1] = rhs.GetUVIndex(1);
		_uvIndices[2] = rhs.GetUVIndex(2);

		_toBeCulled = rhs._toBeCulled;
		_averageZ = rhs._averageZ;
		_polygonNormal = rhs._polygonNormal;

		_RGBValue = rhs._RGBValue;

	}

	return *this;

}

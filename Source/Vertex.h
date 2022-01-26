#pragma once
#include "Vector3D.h"
#include "UVCoord.h"
#include <windows.h>

class Vertex
{
public:

	/*
	Definition of the constructors needed to create an instance
	of a vertex
	*/

	Vertex();
	Vertex(float x, float y, float z, float w);
	Vertex(const Vertex& other);
	
	/*
	Method to dehomogenize the coordinates of a vertex
	*/

	void Dehomogenized();

	/*
	Accessors/Mutators to get and set all XYZW values of a given vertex
	*/

	float GetX() const;
	void SetX(const float x);
	float GetY() const;
	void SetY(const float y);
	float GetZ() const;
	void SetZ(const float z);
	float GetW() const;
	void SetW(const float w);

	/*
	Accessors/Mutators to get and set the normal, colour and contribution counter of a given vertex
	*/

	Vector3D GetVertexNormal() const;
	void SetVertexNormal(const Vector3D& vertexNormal);
	int GetVertexRGB() const;
	void SetVertexRGB(const int& vertexRGB);
	int GetContributeCount() const;
	void SetContributeCount(const int& contributeCount);

	/*
	Accessor/Mutator to get and set the UV coordinate of a given vertex
	*/

	UVCoord GetUVCoord() const;
	void SetUVCoord(const UVCoord& uvCoord);

	/*
	Accessor to get the XY coordinate of a given vertex as an integer
	*/

	int GetIntX() const;
	int GetIntY() const;

	/*
	Accessor/Mutator to get the pre-transformation z coordinate of a given vertex
	*/

	float GetPreTranZ() const;
	void SetPreTranZ(const float& preTranZ);

	/*
	Accessors/Mutators to get the calculation results necessary for the texture mapping of a given vertex
	*/

	float GetUOZ() const;
	void SetUOZ(const float& uOverZ);
	float GetVOZ() const;
	void SetVOZ(const float& vOverZ);
	float GetZR() const;
	void SetZR(const float& zReciprocal);
	
	/*
	Assignment operator followed by a check operator to see if a vertex is equal to another
	*/

	Vertex& operator= (const Vertex& rhs);

	bool operator== (const Vertex& rhs) const;

	/*
	Operators for vertex addition and subtraction
	*/

	const Vertex operator+ (const Vertex& rhs) const;
	const Vector3D operator- (const Vertex& rhs) const;

private:

	/*
	Members to hold the values of a given vertex throughout the demonstration
	*/

	float _x;
	float _y;
	float _z;
	float _w;

	UVCoord _uvCoord;

	Vector3D _vertexNormal;
	int _vertexRGB;
	int _contributeCount;

	float _preTransformZ;

	float _uOverZ;
	float _vOverZ;
	float _zReciprocal;
};


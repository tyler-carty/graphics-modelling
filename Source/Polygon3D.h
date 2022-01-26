#pragma once
#include "Vector3D.h"
#include <windows.h>

class Polygon3D
{
public:

	/*
	Definition of the constructors needed to create an instance
	of a polygon on the model
	*/

	Polygon3D();
	Polygon3D(int index0, int index1, int index2, int uvIndex0, int uvIndex1, int uvIndex2);
	~Polygon3D();
	Polygon3D(const Polygon3D& p);

	/*
	Accessors to return polygon index/UV coordinate index of specified vertex
	*/

	int GetIndex(int) const;
	int GetUVIndex(int) const;

	/*
	Accesses / mutates the state of the current polygon 
	culling boolean and average Z depth value

	Accesses / mutates the RGB value of the polygon 
	as well as its normal vector
	*/

	bool GetCullState() const;
	void SetCullState(const bool cullState);
	float GetAverageZ() const;
	void SetAverageZ(const float averagePolygonZ);

	COLORREF GetRGBValue() const;
	void SetRGBValue(const COLORREF& RGBValue);
	Vector3D GetPolygonNormal() const;
	void SetPolygonNormal(const Vector3D& vectorNormal);

	Polygon3D& operator= (const Polygon3D& rhs);

private:

	/*
	Members to hold the information about a specific polygon
	*/

	int _indices[3];
	int _uvIndices[3];
	bool _toBeCulled;
	float _averageZ;
	int _RGBValue;

	Vector3D _polygonNormal;

};


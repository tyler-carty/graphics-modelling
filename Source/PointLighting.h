#pragma once
#include "Vector3D.h"
#include "Vertex.h"

class PointLighting
{
public:

	/*
	Definition of the constructors needed to create an instance
	of point lighting on the model
	*/

	PointLighting();
	PointLighting(const int& redLightValue, const int& greenLightValue, const int& blueLightValue, const Vertex& pointPosition, const float& a, const float& b, const float& c);
	~PointLighting();

	/*
	Accesses / mutates the light/position values that the ambient light is giving off
	*/

	int GetRedValue() const;
	void SetRedValue(const int redValue);
	int GetGreenValue() const;
	void SetGreenValue(const int greenValue);
	int GetBlueValue() const;
	void SetBlueValue(const int blueValue);

	/*
	Accesses the attenuation constants needed to define the behaviour of this light
	*/

	float GetValueA() const;
	float GetValueB() const;
	float GetValueC() const;

	Vertex GetPointPosition() const;
	void SetPointPosition(const Vertex pointPosition);

	/*
	Clamps RGB values to between 0-255 so nothing is out of range after calculations
	*/

	static float ClampRGBValues(const float& colourValue);

private:

	/*
	Members to hold the light values, position and attenuation constants of point lighting
	*/

	int _redLightValue;
	int _greenLightValue;
	int _blueLightValue;

	float _a;
	float _b;
	float _c;

	Vertex _pointPosition;

};

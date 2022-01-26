#include "PointLighting.h"

/*
	Constructors needed to create an instance
	of point lighting on the model
*/
PointLighting::PointLighting()
{
	_redLightValue = 0;
	_greenLightValue = 0;
	_blueLightValue = 0;

	_pointPosition = Vertex(0, 0, 0, 1);
}

PointLighting::PointLighting(const int& redLightValue, const int& greenLightValue, const int& blueLightValue, const Vertex& pointPosition, const float& a, const float& b, const float& c)
{
	_redLightValue = redLightValue;
	_greenLightValue = greenLightValue;
	_blueLightValue = blueLightValue;

	_a = a;
	_b = b;
	_c = c;

	_pointPosition = pointPosition;
}

PointLighting::~PointLighting()
{
}

/*
	Accesses / mutates the light/position values that the ambient light is giving off
*/

int PointLighting::GetRedValue() const
{
	return _redLightValue;
}

void PointLighting::SetRedValue(const int redValue)
{
	_redLightValue = redValue;
}

int PointLighting::GetGreenValue() const
{
	return _greenLightValue;
}

void PointLighting::SetGreenValue(const int greenValue)
{
	_greenLightValue = greenValue;
}

int PointLighting::GetBlueValue() const
{
	return _blueLightValue;
}

void PointLighting::SetBlueValue(const int blueValue)
{
	_blueLightValue = blueValue;
}

/*
	Accesses the attenuation constants needed to define the behaviour of this light
*/

float PointLighting::GetValueA() const
{
	return _a;
}

float PointLighting::GetValueB() const
{
	return _b;
}

float PointLighting::GetValueC() const
{
	return _c;
}

Vertex PointLighting::GetPointPosition() const
{
	return _pointPosition;
}

void PointLighting::SetPointPosition(const Vertex pointPosition)
{
	_pointPosition = pointPosition;
}

/*
	Clamps RGB values to between 0-255 so nothing is out of range after calculations
*/

float PointLighting::ClampRGBValues(const float& colourValue)
{
	if (colourValue < 0) {
		return(int(0));
	}
	else if (colourValue > 255) {
		return(int(255));
	}
	else {
		return(colourValue);
	}
}

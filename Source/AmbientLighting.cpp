#include "AmbientLighting.h"

/*
Constructors/Destructors for managing an ambient light instantiation
*/

AmbientLighting::AmbientLighting()
{
	_redLightValue = 0;
	_greenLightValue = 0;
	_blueLightValue = 0;
}

AmbientLighting::AmbientLighting(const int& redLightValue, const int& greenLightValue, const int& blueLightValue)
{
	_redLightValue = redLightValue;
	_greenLightValue = greenLightValue;
	_blueLightValue = blueLightValue;
}

AmbientLighting::~AmbientLighting()
{

}

/*
	Accesses / mutates the light values that the ambient light is giving off
*/

int AmbientLighting::GetRedValue() const
{
	return _redLightValue;
}

void AmbientLighting::SetRedValue(const int redLightValue)
{
	_redLightValue = redLightValue;
}

int AmbientLighting::GetGreenValue() const
{
	return _greenLightValue;
}

void AmbientLighting::SetGreenValue(const int greenLightValue)
{
	_greenLightValue = greenLightValue;
}

int AmbientLighting::GetBlueValue() const
{
	return _blueLightValue;
}

void AmbientLighting::SetBlueValue(const int blueLightValue)
{
	_blueLightValue = blueLightValue;
}

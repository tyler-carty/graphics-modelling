#pragma once
class AmbientLighting
{
public:

	/*
	Definition of the constructors needed to create an instance 
	of ambient lighting on the model
	*/

	AmbientLighting();
	AmbientLighting(const int& redLightValue, const int& greenLightValue, const int& blueLightValue);
	~AmbientLighting();


	/*
	Accesses / mutates the light values that the ambient light is giving off
	*/

	int GetRedValue() const;
	void SetRedValue(const int redLightValue);
	int GetGreenValue() const;
	void SetGreenValue(const int greenLightValue);
	int GetBlueValue() const;
	void SetBlueValue(const int blueLightValue);

private:

	/*
	Members to hold the light values of ambient lighting
	*/

	int _redLightValue;
	int _greenLightValue;
	int _blueLightValue;

};
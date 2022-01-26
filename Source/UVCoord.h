#pragma once
class UVCoord
{
public:
	
	/*
	Definition of the constructors needed to create an instance
	of a UV coord for the model
	*/

	UVCoord();
	UVCoord(float u, float v);

	/*
	Accesses / mutates the UV values that the current polygon needs
	*/

	int GetIntU() const;
	void SetU(const float u);
	int GetIntV() const;
	void SetV(const float v);

private:
	
	/*
	Members to hold the UV coords
	*/

	float _u;
	float _v;

};
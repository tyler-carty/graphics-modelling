#include "UVCoord.h"

/*
	Constructors needed to create an instance
	of a UV coord for the model
*/
UVCoord::UVCoord()
{
	_u = 0.0f;
	_v = 0.0f;
}

UVCoord::UVCoord(float u, float v)
{
	_u = u;
	_v = v;
}

/*
	Accesses / mutates the UV values that the current polygon needs
*/

int UVCoord::GetIntU() const
{
	return (int)_u;
}

void UVCoord::SetU(const float u)
{
	_u = u;
}

int UVCoord::GetIntV() const
{
	return (int)_v;
}

void UVCoord::SetV(const float v)
{
	_v = v;
}

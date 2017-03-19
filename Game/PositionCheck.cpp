#include "PositionCheck.h"

#include"Boid.h"


PositionCheck::PositionCheck()
{

}



PositionCheck::~PositionCheck()
{

}



void PositionCheck::checkPos(float& _axis)
{
	// Negative - Positive
	if (_axis < -zoneSize)
	{
		_axis = zoneSize;
	}

	// Positive - Negative
	if (_axis > zoneSize)
	{
		_axis = -zoneSize;
	}
}
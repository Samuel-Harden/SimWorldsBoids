#include "BoidData.h"



BoidData::BoidData()
	: sepWeight(0.5f),
	runWeight(0.4f),
	aliWeight(1.0f),
	cohWeight(1.0f),
	boidMaxSpeed(0.5f),
	boidMaxForce(0.03f),
	neighbourDist(30.0f),
	desiredSep(2.5f)
{

}



BoidData::~BoidData()
{

}
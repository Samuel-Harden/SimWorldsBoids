#include "BoidData.h"



BoidData::BoidData()
	  : sepWeight(0.5f),
	     ffWeight(0.4f),
	    runWeight(0.2f),
	    aliWeight(1.0f),
	    cohWeight(1.0f),
     boidMaxSpeed(0.5f),
    boidMaxForce(0.03f),
   neighbourDist(30.0f),
	   desiredSep(5.0f)
{

}



BoidData::~BoidData()
{

}
#pragma once

#include <d3d11.h>
#include "SimpleMath.h"

class BoidData
{
public:

	// Friend classes have access to protected data
	friend class Boid;
	friend class BoidManager;

	friend class Separation;
	friend class Alignment;
	friend class Cohesion;
	friend class Avoidance;
	friend class Homing;

	BoidData();
	~BoidData();

protected:

	// Behaviour Weights
	float sepWeight;
	float ffWeight;
	float aliWeight;
	float cohWeight;
	float runWeight;

	// Boid Variables
	float boidMaxSpeed;
	float boidMaxForce;
	float neighbourDist;
	float desiredSep;

	DirectX::SimpleMath::Vector3 predatorPos;

private:

};
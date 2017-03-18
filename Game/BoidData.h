#pragma once

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

private:

};
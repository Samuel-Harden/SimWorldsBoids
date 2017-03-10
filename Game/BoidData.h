#pragma once

class BoidData
{
public:

	// Friend classes have acess to protected data
	friend class BoidPrey;
	friend class BoidManager;

	BoidData();
	~BoidData();

protected:

	// Behaviour Weights
	float sepWeight;
	float runWeight;
	float aliWeight;
	float cohWeight;

	// Boid Variables
	float boidSpeed;
	float neighbourDist;
	float desiredSep;

private:

};
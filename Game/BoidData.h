#pragma once

class BoidData
{
public:
	friend class BoidPrey;
	friend class BoidManager;

	BoidData();
	~BoidData();

protected:
	int updateGroup;
	float sepWeight;
	float runWeight;
	float aliWeight;
	float cohWeight;
	float boidSpeed;
	float neighbourDist;
	float desiredSep;

private:

};
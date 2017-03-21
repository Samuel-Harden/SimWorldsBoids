#pragma once

#include <d3d11.h>
#include "SimpleMath.h"
#include <memory>
#include <string>

class BoidData
{
public:

	// Friend classes have access to protected data
	friend class Boid;
	friend class BoidManager;
	friend class FileReader;

	friend class Separation;
	friend class Alignment;
	friend class Cohesion;
	friend class Avoidance;
	friend class PathFinding;

	BoidData(std::unique_ptr<FileReader>& _fileReader, std::string& _fileName);
	~BoidData();

protected:

	// Behaviour Weights
	float sepWeight;
	float ffWeight;
	float aliWeight;
	float cohWeight;
	float runWeight;
	float pathWeight;

	// Boid Variables
	float boidMaxSpeed;
	float boidMaxForce;
	float neighbourDist;
	float desiredSep;

	DirectX::SimpleMath::Vector3 predatorPos;

private:

};
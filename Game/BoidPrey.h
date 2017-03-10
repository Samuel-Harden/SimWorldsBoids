#pragma once

#include "VBGO.h"
#include "vertex.h"
#include <vector>

class BoidData;

// Behaviours
class Separation;
class Alignment;
class Cohesion;

//=================================================================
//procedurally generate a VBGO BoidPrey
//each side be divided in to _size * _size squares (2 triangles per square)
//=================================================================

class BoidPrey : public VBGO
{
public:
	BoidPrey(int& _ID);
	virtual ~BoidPrey();

	//initialise the Veretx and Index buffers for the cube
	void init(ID3D11Device* _GD);

	void Tick(GameData* _GD);
	void run(std::vector<BoidPrey*>& boids, GameData* _GD, int _boidGroup, BoidData* _boidData);

	Vector3 getVelocity();

	bool isActive;
	void activateBoid();
	void deactivateBoid();

	float getRed();
	float getBlue();
	float getGreen();

protected:
	//this is to allow custom versions of this which create the basic cube and then distort it
	//see VBSpiral, VBSpiked and VBPillow
	virtual void Transform() {};

	int m_size;
	myVertex* m_vertices;

private:

	void flock(std::vector<BoidPrey*>& _boids, GameData* _GD, BoidData*& _boidData);

	void applyForce(Vector3& force);

	void checkPosition();
	bool limitPosition();
	void setRandPos();

	// Helper Function
	float randomFloat(float _min, float _max);

	bool newPos;

	Vector3 position;
	Vector3 velocity;
	Vector3 acceleration;

	float maxForce;
	float maxSpeed;
	float neighbourDistance;
	float desiredSeperation;

	int boidID;

	float boidRed;	  // Red colour Value for Vertices
	float boidBlue;	  // Blue colour Value for Vertices
	float boidGreen;  // Green colour Value for Vertices

	// Behaviours
	Vector3 seek(Vector3& target);
	Vector3 seperate(std::vector<BoidPrey*>& _boids);
	Vector3 avoidPredator(GameData* _GD);
	Vector3 align(std::vector<BoidPrey*>& _boids);
	Vector3 cohesion(std::vector<BoidPrey*>& _boids);

	void groupColor(std::vector<BoidPrey*>& _boids);

	std::unique_ptr<Separation> m_separation;
	std::unique_ptr<Alignment> m_alignment;
	std::unique_ptr<Cohesion> m_cohesion;

	// Constants
	const int zoneSize = 200;
};
#pragma once

#include "VBGO.h"
#include "vertex.h"
#include <vector>

//=================================================================
//procedurally generate a VBGO BoidPrey
//each side be divided in to _size * _size squares (2 triangles per square)
//=================================================================

class PredatorBoid : public VBGO
{
public:
	PredatorBoid();
	virtual ~PredatorBoid();

	//initialise the Veretx and Index buffers for the cube
	void init(ID3D11Device* _GD);

	void Tick(GameData* _GD);
	void run(std::vector<PredatorBoid*>& boids, GameData* _GD);

	Vector3 getVelocity();

	bool isActive;
	void activateBoid();

protected:
	//this is to allow custom versions of this which create the basic cube and then distort it
	//see VBSpiral, VBSpiked and VBPillow
	virtual void Transform() {};

	int m_size;
	myVertex* m_vertices;

private:

	void flock(std::vector<PredatorBoid*>& _boids);
	void applyForce(Vector3& force);

	void checkPosition();

	float randomFloat(float _min, float _max);

	Vector3 position;
	Vector3 velocity;
	Vector3 acceleration;
	Vector3 m_up;

	float maxForce;
	float maxSpeed;

	int vert;

	int zoneSize;

	float neighbourDistance;
	float desiredSeperation;
	float boidRed;
	float boidBlue;
	float boidGreen;

	// Behaviours
	Vector3 seek(Vector3& target);
	Vector3 seperate(std::vector<PredatorBoid*>& _boids);
	Vector3 align(std::vector<PredatorBoid*>& _boids);
	Vector3 cohesion(std::vector<PredatorBoid*>& _boids);
};
#pragma once

#include "VBGO.h"
#include "vertex.h"
#include <vector>

//=================================================================
//procedurally generate a VBGO BoidPrey
//each side be divided in to _size * _size squares (2 triangles per square)
//=================================================================

class VBBoidPrey : public VBGO
{
public:
	VBBoidPrey();
	virtual ~VBBoidPrey();

	//initialise the Veretx and Index buffers for the cube
	void init(ID3D11Device* _GD);

	void Tick(GameData* _GD);
	void run(std::vector<VBBoidPrey*>& boids, GameData* _GD);

	Vector3 getVelocity();

	bool isActive;
	void activateBoid();

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

	void flock(std::vector<VBBoidPrey*>& _boids);
	void applyForce(Vector3& force);

	void groupColor(std::vector<VBBoidPrey*>& _boids);

	void checkPosition();

	float randomFloat(float _min, float _max);

	Vector3 position;
	Vector3 velocity;
	Vector3 acceleration;

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
	Vector3 seperate(std::vector<VBBoidPrey*>& _boids);
	Vector3 align(std::vector<VBBoidPrey*>& _boids);
	Vector3 cohesion(std::vector<VBBoidPrey*>& _boids);

	Vector3 m_dir;
	Vector3 m_up;
	Vector3 direction;
	Vector3 desired;

	Vector3 m_target;

	Matrix m_viewMat;
};
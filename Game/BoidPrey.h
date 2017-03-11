#pragma once

#include "VBGO.h"
#include "vertex.h"
#include <vector>

class BoidData;

// Behaviours
class Separation;
class Alignment;
class Cohesion;
class Avoidance;



class BoidPrey : public VBGO
{
public:
	BoidPrey(int& _ID, int& _faction);
	virtual ~BoidPrey();

	//initialise the Veretx and Index buffers for the cube
	void init(ID3D11Device* _GD);

	void Tick(GameData* _GD, BoidData* _BD);
	void run(std::vector<BoidPrey*>& boids, GameData* _GD, int _boidGroup, BoidData* _boidData);

	Vector3 getVelocity() const;

	bool isActive;
	void activateBoid();
	void deactivateBoid();

	int getFaction();

protected:
	//this is to allow custom versions of this which create the basic cube and then distort it
	//see VBSpiral, VBSpiked and VBPillow
	virtual void Transform() {};

	int m_size;
	myVertex* m_vertices;

private:

	void flock(std::vector<BoidPrey*>& _boids, GameData* _GD, BoidData*& _boidData);

	void applyForce(Vector3& force);

	void setFactionColour();

	void checkPosition();
	void setRandPos();

	// Helper Function
	float randomFloat(float _min, float _max);

	bool newPos;

	Vector3 velocity;
	Vector3 acceleration;

	int boidID;
	int faction;

	float boidRed;	  // Red colour Value for Vertices
	float boidBlue;	  // Blue colour Value for Vertices
	float boidGreen;  // Green colour Value for Vertices

	std::unique_ptr<Separation> m_separation;
	std::unique_ptr<Alignment> m_alignment;
	std::unique_ptr<Cohesion> m_cohesion;
	std::unique_ptr<Avoidance> m_avoidance;

	// Constants
	const int zoneSize = 200;
};
#pragma once

#include "VBGO.h"
#include "vertex.h"
#include <vector>

class BoidData;
class Behaviour;



class Boid : public VBGO
{
public:
	Boid(int& _ID, int& _faction);
	virtual ~Boid();

	//initialise the Veretx and Index buffers for the cube
	void init(ID3D11Device* _GD);

	void Tick(GameData* _GD, BoidData* _BD);
	void run(std::vector<Boid*>& boids, GameData* _GD, int _boidGroup, BoidData* _boidData, std::vector<Behaviour*> _behaviours);

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

	void flock(std::vector<Boid*>& _boids, GameData* _GD, BoidData*& _boidData,
		std::vector<Behaviour*> _behaviours);

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

	// Constants
	const int zoneSize = 200;
};
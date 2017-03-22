#pragma once

#include "VBGO.h"
#include "vertex.h"
#include <vector>

class BoidData;
class Behaviour;
class PositionCheck;



class Boid : public VBGO
{
public:
	Boid(int& _ID, int& _faction);
	virtual ~Boid();

	//initialise the Veretx and Index buffers for the Boid
	void init(ID3D11Device* _GD);

	void Tick(BoidData* _BD);
	void run(std::vector<Boid*>& boids, int _boidGroup,
		BoidData* _boidData, std::vector<Behaviour*> _behaviours,
		PositionCheck*& _posCheck, std::vector<Vector3>& _wpPos);

	Vector3 getVelocity() const;
	int getFaction() const;

	void setWayPointID(int& _newWayPoint);
	int getWayPointID() const;

	bool isActive;
	void activateBoid();
	void deactivateBoid();

protected:

	int m_size;
	myVertex* m_vertices;

private:

	// Functions
	void flock(std::vector<Boid*>& _boids, BoidData*& _boidData,
		std::vector<Behaviour*> _behaviours, std::vector<Vector3>& _wpPos);

	void applyForce(Vector3& force);

	void setFactionColour();
	void setRandPos();

	// Helper Function
	float randomFloat(float _min, float _max);

	// Variables
	bool newPos;

	Vector3 velocity;
	Vector3 acceleration;

	int boidID;
	int faction;
	int wayPointID;

	float boidRed;	  // Red colour Value for Vertices
	float boidBlue;	  // Blue colour Value for Vertices
	float boidGreen;  // Green colour Value for Vertices

	// Constants
	const int zoneSize = 200;
};
#include "Avoidance.h"

#include"BoidPrey.h"
#include "BoidData.h"
#include "GameData.h"



Avoidance::Avoidance()
{

}



Avoidance::~Avoidance()
{

}



// Avoid
// Method checks for nearby predator and steers away
Vector3 Avoidance::avoid(BoidPrey* _boid, BoidData* _BD, GameData* _GD)
{
	Vector3 steer = Vector3::Zero;
	int count = 0;

	float d = Vector3::Distance(_boid->getPos(), _GD->predatorPos);

	// if Boid is a neighbour
	if (d > 0 && d < (_BD->neighbourDist * 5))
	{
		// Calculate vector pointing away from neighbour
		Vector3 diff = (_boid->getPos() - (_GD->predatorPos));
		diff = XMVector3Normalize(diff);
		diff = (diff / d); // Weight by distance
		steer = (steer + diff);
	}

	// As long as the vector is greater than 0
	if (steer != Vector3::Zero)
	{
		steer = XMVector3ClampLength(steer, 0.0f, _BD->boidMaxSpeed);

		// Implement Reynolds: steering = desired - velocity
		steer = XMVector3Normalize(steer);
		steer = (steer * _BD->boidMaxSpeed);
		steer = (steer - _boid->getVelocity());

		steer = XMVector3ClampLength(steer, 0.0f, (_BD->boidMaxSpeed * 2));
	}
	return steer;
}
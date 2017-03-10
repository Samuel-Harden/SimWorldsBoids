#include "Separation.h"

#include"BoidPrey.h"
#include "BoidData.h"



Separation::Separation()
{

}



Separation::~Separation()
{

}



// Seperation
// Method checks for nearby boids and steers away
Vector3 Separation::separate(BoidPrey* _boid, BoidData* _BD, std::vector<BoidPrey*>& _boids)
{
	Vector3 steer = Vector3::Zero;
	int count = 0;

	// Check through every other boid
	for (int i = 0; i < _boids.size(); i++)
	{
		if (_boids[i]->isActive == true)
		{
			float d = Vector3::Distance(_boid->getPos(), _boids[i]->getPos());

			// if Boid is a neighbour
			if (d > 0 && d < _BD->desiredSep)
			{
				// Calculate vector pointing away from neighbour
				Vector3 diff = (_boid->getPos() - _boids[i]->getPos());
				diff = XMVector3Normalize(diff);
				diff = (diff / d); // Weight by distance
				steer = (steer + diff);
				count++;
			}
		}
	}

	// Average -- divided by how many
	if (count > 0)
	{
		steer = (steer / count);
	}

	// As long as the vector is greater than 0
	if (steer != Vector3::Zero)
	{
		steer = XMVector3ClampLength(steer, 0.0f, _BD->boidMaxSpeed);

		// Implement Reynolds: steering = desired - velocity
		steer = XMVector3Normalize(steer);
		steer = (steer * _BD->boidMaxSpeed);
		steer = (steer - _boid->getVelocity());

		steer = XMVector3ClampLength(steer, 0.0f, _BD->boidMaxSpeed);
	}
	return steer;
}
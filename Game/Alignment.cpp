#include "Alignment.h"

#include"BoidPrey.h"
#include "BoidData.h"



Alignment::Alignment()
{

}



Alignment::~Alignment()
{

}



// Alignment
// For every nearby boid in the system, calculate the average velocity
Vector3 Alignment::align(BoidPrey* _boid, BoidData* _BD, std::vector<BoidPrey*>& _boids)
{
	// The position the boid wants to be
	Vector3 sum = Vector3::Zero;
	int count = 0;

	// Check through every other boid
	for (int i = 0; i < _boids.size(); i++)
	{
		if (_boids[i]->isActive == true)
		{
			float d = Vector3::Distance(_boid->getPos(), _boids[i]->getPos());

			// if Boid is a neighbour
			if (d > 0 && d < _BD->neighbourDist)
			{
				sum = (sum + _boids[i]->getVelocity());
				count++;
			}
		}
	}

	if (count > 0)
	{
		sum = sum / count;

		sum = XMVector3ClampLength(sum, 0.0f, _BD->boidMaxSpeed);

		// Implement Reynolds: steering = desired - velocity
		sum = XMVector3Normalize(sum);
		sum = (sum * _BD->boidMaxSpeed);
		Vector3 steer = (sum - _boid->getVelocity());

		steer = XMVector3ClampLength(steer, 0.0f, _BD->boidMaxForce);

		return steer;
	}
	else
		return Vector3::Zero;
}
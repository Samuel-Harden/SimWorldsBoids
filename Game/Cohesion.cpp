#include "Cohesion.h"

#include"Boid.h"
#include "BoidData.h"



Cohesion::Cohesion()
{

}



Cohesion::~Cohesion()
{

}


// Cohesion
// For the average position (I.E. center) of all nearby boids,
// calculate towards that position
Vector3 Cohesion::calculateBehaviour(Boid* _boid, BoidData* _BD, std::vector<Boid*>& _boids)
{
	Vector3 sum = Vector3::Zero; // start with an empty vector to accumulate all positions
	int count = 0;

	for (int i = 0; i < _boids.size(); i++)
	{
		if (_boids[i]->isActive == true)
		{
			float d = Vector3::Distance(_boid->getPos(), _boids[i]->getPos());

			// if Boid is a neighbour
			if (d > 0 && d < _BD->neighbourDist)
			{
				sum = (sum + _boids[i]->getPos());
				count++;
			}
		}
	}

	if (count > 0)
	{
		sum = (sum / count);

		return (seek(sum, _boid, _BD)); // steer towards the position
	}
	else
		return Vector3::Zero;
}



// STEER = DESIRED MINUS VELOCITY
Vector3 Cohesion::seek(Vector3& target, Boid* _boid, BoidData* _BD)
{
	// A Vector pointing from the position to the target
	Vector3 desired = (target - _boid->getPos());

	// Scale to max Speed
	desired = XMVector3Normalize(desired);
	desired = (desired * _BD->boidMaxSpeed);

	desired = XMVector3ClampLength(desired, 0.0f, _BD->boidMaxSpeed);

	// Steering = desired - velocity
	Vector3 steer = (desired - _boid->getVelocity());

	steer = XMVector3ClampLength(steer, 0.0f, _BD->boidMaxForce);

	return steer;
}
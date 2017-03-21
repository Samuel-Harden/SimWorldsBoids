#include "PathFinding.h"

#include"Boid.h"
#include "BoidData.h"
#include "GameData.h"



PathFinding::PathFinding()
{

}



PathFinding::~PathFinding()
{

}



Vector3 PathFinding::calculateBehaviour3(Boid* _boid, BoidData* _BD, std::vector<Vector3>& _wpPos)
{
	Vector3 steer = Vector3::Zero;
	int count = 0;

	float d = Vector3::Distance(_boid->getPos(), _wpPos[_boid->getWayPointID()]);

	// if Boid is further than 10 away from waypoint
	if (d > 25)
	{
		// Calculate vector pointing away from neighbour
		Vector3 diff = (_boid->getPos() - (_wpPos[_boid->getWayPointID()]));
		diff = XMVector3Normalize(diff);
		diff = (diff / d); // Weight by distance
		steer = (steer + diff);
	}

	if (d < 25)
	{
		for (int i = 0; i < _wpPos.size(); i++)
		{
			if (_boid->getWayPointID() == i)
			{
				i++;
				if (i == (_wpPos.size()))
				{
					i = 0;
				}
				_boid->setWayPointID(i); // assign next Waypoint...
				break;
			}
		}
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
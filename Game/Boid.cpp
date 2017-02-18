#include "Boid.h"
#include "GameData.h"
#include <math.h>
#include "SimpleMath.h"
#include <vector>

Boid::Boid(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF) : CMOGO(_fileName, _pd3dDevice, _EF)
{
	desiredSeperation = 10.0f;

	acceleration = Vector3::Zero;

	velocity.x = randomFloat(0.5f, 2.0f);
	velocity.y = randomFloat(0.5f, 2.0f);
	velocity.z = randomFloat(0.5f, 2.0f);

	float angle = randomFloat(0.5f, 6.2f);
	float angle1 = cos(angle);
	float angle2 = sin(angle);
	float angle3 = tan(angle);

	velocity = Vector3(angle1, angle2, angle3);

	maxSpeed = 0.2f;
	maxForce = 0.03f;

	neighbourDistance = 20.0f;

	m_pos = Vector3::Zero;

	isActive = false;
}



Boid::~Boid()
{

}



void Boid::activateBoid()
{
	isActive = true;
}



void Boid::run(std::vector<Boid*>& _boids, GameData* _GD)
{
	flock(_boids);
	Tick(_GD);
}



void Boid::flock(std::vector<Boid*>& _boids)
{
	Vector3 sep = seperate(_boids);  // Seperation
	Vector3 ali = align(_boids);     // Alignment
	Vector3 coh = cohesion(_boids);  // Cohesion

	// Arbitrarily weight these force
	sep *= 1.0f;
	ali *= 2.0f;
	coh *= 1.0f;
	applyForce(sep);
	applyForce(ali);
	applyForce(coh);
}



void Boid::applyForce(Vector3& force)
{
	acceleration += force;
}



void Boid::Tick(GameData* _GD)
{
	// update velocity
	velocity += acceleration;

	velocity.Normalize();
	velocity = XMVector3ClampLength(velocity, 0.0f, maxSpeed);

	m_pos += velocity;

	//transform.rotation = Quaternion.LookRotation(velocity);

	// reset acceleration to 0 each cycle
	acceleration = Vector3::Zero;

	CMOGO::Tick(_GD);
}



// STEER = DESIRED MINUS VELOCITY
Vector3 Boid::seek(Vector3& target)
{ 
	// A Vector pointing from the position to the target
	Vector3 desired = (target - m_pos);

	// Scale to max Speed
	desired.Normalize();
	desired *= maxSpeed;

	desired.Normalize();
	desired = XMVector3ClampLength(desired, 0.0f, maxSpeed);

	// Steering = desired - velocity
	Vector3 steer = (desired - velocity);

	steer.Normalize();
	steer = XMVector3ClampLength(steer, 0.0f, maxForce);

	return steer;
}


// Seperation
// Method checks for nearby boids and steers away
Vector3 Boid::seperate(std::vector<Boid*> _boids)
{
	Vector3 steer = Vector3::Zero;
	int count = 0;

	// Check through every other boid
	for (int i = 0; i < _boids.size(); i++)
	{
		if (_boids[i]->isActive == true)
		{
			float d = Vector3::Distance(m_pos, _boids[i]->GetPos());

			// if Boid is a neighbour
			if (d > 0 && d < desiredSeperation)
			{
				// Calculate vector pointing away from neighbour
				Vector3 diff = (m_pos - _boids[i]->GetPos());
				diff.Normalize();
				diff /= d; // Weight by distance
				steer += diff;
				count++;
			}
		}
	}

	// Average -- divided by how many
	if (count > 0)
	{
		steer /= count;
	}

	// As long as the vector is greater than 0
	if (steer != Vector3::Zero)
	{
		steer = XMVector3ClampLength(steer, 0.0f, maxSpeed);

		// Implement Reynolds: steering = desired - velocity
		steer.Normalize();
		steer *= maxSpeed;
		steer -= velocity;

		steer.Normalize();
		steer = XMVector3ClampLength(steer, 0.0f, maxSpeed);
	}
	return steer;
}


// Alignment
// For every nearby boid in the system, calculate the average velocity
Vector3 Boid::align(std::vector<Boid*> _boids)
{
	// The position the boid wants to be
	Vector3 sum = Vector3::Zero;
	int count = 0;

	// Check through every other boid
	for (int i = 0; i < _boids.size(); i++)
	{
		if (_boids[i]->isActive == true)
		{
			float d = Vector3::Distance(m_pos, _boids[i]->GetPos());

			// if Boid is a neighbour
			if (d > 0 && d < neighbourDistance)
			{
				sum += _boids[i]->getVelocity();
				count++;
			}
		}
	}

	if (count > 0)
	{
		sum /= count;

		sum.Normalize();
		sum = XMVector3ClampLength(sum, 0.0f, maxSpeed);

		// Implement Reynolds: steering = desired - velocity
		sum.Normalize();
		sum *= maxSpeed;
		Vector3 steer = (sum - velocity);

		steer.Normalize();
		steer = XMVector3ClampLength(steer, 0.0f, maxForce);

		return steer;
	}
	else
		return Vector3::Zero;
}


// Cohesion
// For the average position (I.E. center) of all nearby boids,
// calculate towards that position
Vector3 Boid::cohesion(std::vector<Boid*> _boids)
{
	Vector3 sum = Vector3::Zero; // start with an empty vector to accumulate all positions
	int count = 0;

	for (int i = 0; i < _boids.size(); i++)
	{
		if (_boids[i]->isActive == true)
		{
			float d = Vector3::Distance(m_pos, _boids[i]->GetPos());

			// if Boid is a neighbour
			if (d > 0 && d < neighbourDistance)
			{
				sum += _boids[i]->GetPos();
				count++;
			}
		}
	}

	if (count > 0)
	{
		sum /= count;

		return seek(sum); // steer towards the position
	}
	else
		return Vector3::Zero;
}



Vector3 Boid::getVelocity()
{
	return velocity;
}



float Boid::randomFloat(float _min, float _max)
{
	float r = (float)rand() / (float)RAND_MAX;
	return _min + r * (_max - _min);
}
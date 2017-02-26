#include "VBBoidPrey.h"


VBBoidPrey::VBBoidPrey()
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

	maxSpeed = 0.3f;
	maxForce = 0.03f;

	neighbourDistance = 25.0f;

	m_pos = Vector3::Zero;

	isActive = false;
}

void VBBoidPrey::init(int _size, ID3D11Device* GD)
{
	m_size = _size;

	//calculate number of vertices and primatives
	int numVerts = 12;
	m_numPrims = numVerts / 3;
	m_vertices = new myVertex[numVerts];
	WORD* indices = new WORD[numVerts];

	//as using the standard VB shader set the tex-coords somewhere safe
	for (int i = 0; i<numVerts; i++)
	{
		indices[i] = i;
		m_vertices[i].texCoord = Vector2::One;
	}

	//in each loop create the two traingles for the matching sub-square on each of the six faces
	int vert = 0;

	m_vertices[vert].Color = Color(0.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[vert++].Pos = Vector3(0.0f, 0.0f, 0.0f);
	m_vertices[vert].Color = Color(0.0f, 1.0f, 1.0f, 1.0f); // BottomFace 
	m_vertices[vert++].Pos = Vector3(5.0f, 0.f, 0.0f);
	m_vertices[vert].Color = Color(0.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[vert++].Pos = Vector3(2.5f, 0.0f, -5.0f);

	m_vertices[vert].Color = Color(0.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[vert++].Pos = Vector3(0.0f, 0.0f, 0.0f);
	m_vertices[vert].Color = Color(0.0f, 1.0f, 1.0f, 1.0f); // FlatFace
	m_vertices[vert++].Pos = Vector3(5.0f, 0.0f, 0.0f);
	m_vertices[vert].Color = Color(0.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[vert++].Pos = Vector3(2.5f, 1.25f, 0.0f);

	m_vertices[vert].Color = Color(0.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[vert++].Pos = Vector3(2.5f, 0.0f, -5.0f);
	m_vertices[vert].Color = Color(0.0f, 1.0f, 1.0f, 1.0f); // Face2
	m_vertices[vert++].Pos = Vector3(0.0f, 0.0f, 0.0f);
	m_vertices[vert].Color = Color(0.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[vert++].Pos = Vector3(2.5f, 1.25f, 0.0f);

	m_vertices[vert].Color = Color(0.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[vert++].Pos = Vector3(5.0f, 0.0f, 0.0f);
	m_vertices[vert].Color = Color(0.0f, 1.0f, 1.0f, 1.0f); // Face3
	m_vertices[vert++].Pos = Vector3(2.5f, 0.0f, -5.0f);
	m_vertices[vert].Color = Color(0.0f, 1.0f, 1.0f, 1.0f);
	m_vertices[vert++].Pos = Vector3(2.5f, 1.25f, 0.0f);

	//carry out some kind of transform on these vertices to make this object more interesting
	Transform();

	//calculate the normals for the basic lighting in the base shader
	for (int i = 0; i<m_numPrims; i++)
	{
		WORD V1 = 3 * i;
		WORD V2 = 3 * i + 1;
		WORD V3 = 3 * i + 2;

		//build normals
		Vector3 norm;
		Vector3 vec1 = m_vertices[V1].Pos - m_vertices[V2].Pos;
		Vector3 vec2 = m_vertices[V3].Pos - m_vertices[V2].Pos;
		norm = vec1.Cross(vec2);
		norm.Normalize();

		m_vertices[V1].Norm = norm;
		m_vertices[V2].Norm = norm;
		m_vertices[V3].Norm = norm;
	}


	BuildIB(GD, indices);
	BuildVB(GD, numVerts, m_vertices);

	delete[] indices;    //this is no longer needed as this is now in the index Buffer
	delete[] m_vertices; //this is no longer needed as this is now in the Vertex Buffer
	m_vertices = nullptr;
}



void VBBoidPrey::activateBoid()
{
	isActive = true;
}



void VBBoidPrey::run(std::vector<VBBoidPrey*>& _boids, GameData* _GD)
{
	flock(_boids);
	Tick(_GD);
}



void VBBoidPrey::flock(std::vector<VBBoidPrey*>& _boids)
{
	Vector3 sep = seperate(_boids);  // Seperation
	Vector3 ali = align(_boids);     // Alignment
	Vector3 coh = cohesion(_boids);  // Cohesion

									 // Arbitrarily weight these force
	sep *= 1.5f;
	ali *= 1.0;
	coh *= 1.0f;
	applyForce(sep);
	applyForce(ali);
	applyForce(coh);
}



void VBBoidPrey::applyForce(Vector3& force)
{
	acceleration = (acceleration + force);
}



void VBBoidPrey::Tick(GameData* _GD)
{
	// update velocity
	velocity = (velocity + acceleration);

	velocity = XMVector3Normalize(velocity);

	velocity = XMVector3ClampLength(velocity, 0.0f, maxSpeed);
	//velocity.Clamp(Vector3(0.0f,0.0f,0.0f), Vector3(0.2f,0.2f,0.2f));

	m_pos = (m_pos + velocity);

	//transform.rotation = Quaternion.LookRotation(velocity);

	// reset acceleration to 0 each cycle
	acceleration = Vector3::Zero;

	VBGO::Tick(_GD);
}



// STEER = DESIRED MINUS VELOCITY
Vector3 VBBoidPrey::seek(Vector3& target)
{
	// A Vector pointing from the position to the target
	Vector3 desired = (target - m_pos);

	// Scale to max Speed
	desired = XMVector3Normalize(desired);
	desired = (desired * maxSpeed);

	desired = XMVector3ClampLength(desired, 0.0f, maxSpeed);

	// Steering = desired - velocity
	Vector3 steer = (desired - velocity);

	steer = XMVector3ClampLength(steer, 0.0f, maxForce);

	return steer;
}


// Seperation
// Method checks for nearby boids and steers away
Vector3 VBBoidPrey::seperate(std::vector<VBBoidPrey*>& _boids)
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
		steer = XMVector3ClampLength(steer, 0.0f, maxSpeed);

		// Implement Reynolds: steering = desired - velocity
		steer = XMVector3Normalize(steer);
		steer = (steer * maxSpeed);
		steer = (steer - velocity);

		steer = XMVector3ClampLength(steer, 0.0f, maxSpeed);
	}
	return steer;
}


// Alignment
// For every nearby boid in the system, calculate the average velocity
Vector3 VBBoidPrey::align(std::vector<VBBoidPrey*>& _boids)
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
				sum = (sum + _boids[i]->getVelocity());
				count++;
			}
		}
	}

	if (count > 0)
	{
		sum = sum / count;

		sum = XMVector3ClampLength(sum, 0.0f, maxSpeed);

		// Implement Reynolds: steering = desired - velocity
		sum = XMVector3Normalize(sum);
		sum = (sum * maxSpeed);
		Vector3 steer = (sum - velocity);

		steer = XMVector3ClampLength(steer, 0.0f, maxForce);

		return steer;
	}
	else
		return Vector3::Zero;
}


// Cohesion
// For the average position (I.E. center) of all nearby boids,
// calculate towards that position
Vector3 VBBoidPrey::cohesion(std::vector<VBBoidPrey*>& _boids)
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
				sum = (sum + _boids[i]->GetPos());
				count++;
			}
		}
	}

	if (count > 0)
	{
		sum = (sum / count);

		return seek(sum); // steer towards the position
	}
	else
		return Vector3::Zero;
}



Vector3 VBBoidPrey::getVelocity()
{
	return velocity;
}



float VBBoidPrey::randomFloat(float _min, float _max)
{
	float r = (float)rand() / (float)RAND_MAX;
	return _min + r * (_max - _min);
}
#include "Boid.h"

#include "GameData.h"
#include "BoidData.h"

#include "Behaviour.h"


Boid::Boid(int& _ID, int& _faction)
	             : boidID (_ID),
	          faction(_faction),
	            isActive(false),
	              newPos(false),
	acceleration(Vector3::Zero)
{

	float angle  = randomFloat(0.5f, 6.2f);
	float angle1 = cos(angle);
	float angle2 = sin(angle);
	float angle3 = tan(angle);

	velocity = Vector3(angle1, angle2, angle2);

	// Set random starting positon, inside the Designated zone
	setRandPos();
}



Boid::~Boid()
{
	
}



void Boid::init(ID3D11Device* GD)
{
	// Set boids colour based on its faction
	setFactionColour();

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

	int vert = 0;

	m_vertices[vert].Color = Color(boidRed, boidGreen, boidBlue, 1.0f);
	m_vertices[vert++].Pos = Vector3(0.0f, 0.0f, 0.0f);
	m_vertices[vert].Color = Color(boidRed, boidGreen, boidBlue, 1.0f); // BottomFace 
	m_vertices[vert++].Pos = Vector3(5.0f, 0.f, 0.0f);
	m_vertices[vert].Color = Color(boidRed, boidGreen, boidBlue, 1.0f);
	m_vertices[vert++].Pos = Vector3(2.5f, 0.0f, 5.0f);

	m_vertices[vert].Color = Color(boidRed, boidGreen, boidBlue, 1.0f);
	m_vertices[vert++].Pos = Vector3(5.0f, 0.0f, 0.0f);
	m_vertices[vert].Color = Color(boidRed, boidGreen, boidBlue, 1.0f); // FlatFace
	m_vertices[vert++].Pos = Vector3(0.0f, 0.0f, 0.0f);
	m_vertices[vert].Color = Color(boidRed, boidGreen, boidBlue, 1.0f);
	m_vertices[vert++].Pos = Vector3(2.5f, 1.25f, 0.0f);

	m_vertices[vert].Color = Color(boidRed, boidGreen, boidBlue, 1.0f);
	m_vertices[vert++].Pos = Vector3(2.5f, 0.0f, 5.0f);
	m_vertices[vert].Color = Color(boidRed, boidGreen, boidBlue, 1.0f); // Face2
	m_vertices[vert++].Pos = Vector3(2.5f, 1.25f, 0.0f);
	m_vertices[vert].Color = Color(boidRed, boidGreen, boidBlue, 1.0f);
	m_vertices[vert++].Pos = Vector3(0.0f, 0.0f, 0.0f);

	m_vertices[vert].Color = Color(boidRed, boidGreen, boidBlue, 1.0f);
	m_vertices[vert++].Pos = Vector3(2.5f, 1.25f, 0.0f);
	m_vertices[vert].Color = Color(boidRed, boidGreen, boidBlue, 1.0f); // Face3
	m_vertices[vert++].Pos = Vector3(2.5f, 0.0f, 5.0f);
	m_vertices[vert].Color = Color(boidRed, boidGreen, boidBlue, 1.0f);
	m_vertices[vert++].Pos = Vector3(5.0f, 0.0f, 0.0f);

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



void Boid::setFactionColour()
{
	switch (faction)
	{
	case 0:
		boidRed = 1.0f;
		boidGreen = 0.0f;
		boidBlue = 0.0f;
		break;

	case 1:
		boidRed = 0.0f;
		boidGreen = 1.0f;
		boidBlue = 0.0f;
		break;
	case 2:

		boidRed = 0.4f;
		boidGreen = 0.0f;
		boidBlue = 1.0f;
		break;
	}
}



void Boid::activateBoid()
{
	isActive = true;
}


void Boid::deactivateBoid()
{
	// Deactivate Boid
	isActive = false;

	// Set new Random Pos
	setRandPos();
}



void Boid::run(std::vector<Boid*>& _boids, GameData* _GD,
	int _boidGroup, BoidData* _boidData, std::vector<Behaviour*> _behaviours)
{
	// If this boid, is in the current group to be updated, update behaviours...
	if (boidID >= _boidGroup && boidID <= (_boidGroup + 100))
	{
		flock(_boids, _GD, _boidData,  _behaviours);
	}

	Tick(_GD, _boidData);

	checkPosition();
}



void Boid::flock(std::vector<Boid*>& _boids, GameData* _GD,
	BoidData*& _boidData, std::vector<Behaviour*> _behaviours)
{
	Vector3 ali   = _behaviours[0]->calculateBehaviour1(this, _boidData, _boids);    // Alignment
	Vector3 avoid = _behaviours[1]->calculateBehaviour1(this, _boidData, _boids);    // Avoidance
	Vector3 pred  = _behaviours[1]->calculateBehaviour2(this, _boidData);
	Vector3 coh   = _behaviours[2]->calculateBehaviour1(this, _boidData, _boids);    // Cohesion
	Vector3 sep   = _behaviours[3]->calculateBehaviour1(this, _boidData, _boids);    // Seperation

	// Arbitrarily weight these force
	sep *= _boidData->sepWeight;
	avoid *= _boidData->ffWeight;
	ali *= _boidData->aliWeight;
	coh *= _boidData->cohWeight;

	pred *= _boidData->runWeight;

	applyForce(sep);
	applyForce(ali);
	applyForce(coh);
	applyForce(avoid);
	applyForce(pred);
}



void Boid::applyForce(Vector3& force)
{
	acceleration = (acceleration + force);
}



void Boid::Tick(GameData* _GD, BoidData* _BD)
{
	// update velocity
	velocity = (velocity + acceleration);

	velocity = XMVector3Normalize(velocity);

	velocity = XMVector3ClampLength(velocity, 0.0f, _BD->boidMaxSpeed);

	m_pos = (m_pos + velocity);

	Matrix scaleMat = Matrix::CreateScale(m_scale);
	Matrix rotTransMat = Matrix::CreateWorld(m_pos, -velocity, Vector3::Up);

	m_worldMat = m_fudge * scaleMat * rotTransMat;

	// reset acceleration to 0 each cycle
	acceleration = Vector3::Zero;
}



Vector3 Boid::getVelocity() const
{
	return velocity;
}



int Boid::getFaction()
{
	return faction;
}



void Boid::setRandPos()
{
	// Set random starting positon, inside the Designated zone
	float posX = randomFloat(-zoneSize, zoneSize);
	float posY = randomFloat(-zoneSize, zoneSize);
	float posZ = randomFloat(-zoneSize, zoneSize);

	m_pos = Vector3(posX, posY, posZ);
}



float Boid::randomFloat(float _min, float _max)
{
	float r = (float)rand() / (float)RAND_MAX;
	return _min + r * (_max - _min);
}



void Boid::checkPosition()
{
	// Check X axis
	if (m_pos.x < -zoneSize)
	{
		m_pos.x = zoneSize;
	}

	if (m_pos.x > zoneSize)
	{
		m_pos.x = -zoneSize;
	}

	// Check Y axis
	if (m_pos.y < -zoneSize)
	{
		m_pos.y = zoneSize;
	}

	if (m_pos.y > zoneSize)
	{
		m_pos.y = -zoneSize;
	}	
	
	// Check Z axis
	if (m_pos.z < -zoneSize)
	{
		m_pos.z = zoneSize;
	}

	if (m_pos.z > zoneSize)
	{
		m_pos.z = -zoneSize;
	}
}
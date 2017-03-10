#include "BoidPrey.h"

#include "GameData.h"
#include "BoidData.h"

#include "Alignment.h"
#include "Separation.h"
#include "Cohesion.h"
#include "Avoidance.h"


BoidPrey::BoidPrey(int& _ID)
	             : boidID (_ID),
	             maxSpeed(0.5f),
	            maxForce(0.03f),
	   neighbourDistance(30.0f),
	    desiredSeperation(2.5f),
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

	// Create Behaviours
	m_alignment  = std::make_unique<Alignment>();
	m_separation = std::make_unique<Separation>();
	m_cohesion   = std::make_unique<Cohesion>();
	m_avoidance = std::make_unique<Avoidance>();
}



BoidPrey::~BoidPrey()
{
	delete m_vertices;
	m_vertices = nullptr;
}



void BoidPrey::init(ID3D11Device* GD)
{
	boidRed = randomFloat(0.0f, 1.0f);
	boidBlue = randomFloat(0.0f, 1.0f);
	boidGreen = randomFloat(0.0f, 1.0f);

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

	m_vertices[vert].Color = Color(boidRed, boidBlue, boidGreen, 1.0f);
	m_vertices[vert++].Pos = Vector3(0.0f, 0.0f, 0.0f);
	m_vertices[vert].Color = Color(boidRed, boidBlue, boidGreen, 1.0f); // BottomFace 
	m_vertices[vert++].Pos = Vector3(5.0f, 0.f, 0.0f);
	m_vertices[vert].Color = Color(boidRed, boidBlue, boidGreen, 1.0f);
	m_vertices[vert++].Pos = Vector3(2.5f, 0.0f, 5.0f);

	m_vertices[vert].Color = Color(boidRed, boidBlue, boidGreen, 1.0f);
	m_vertices[vert++].Pos = Vector3(5.0f, 0.0f, 0.0f);
	m_vertices[vert].Color = Color(boidRed, boidBlue, boidGreen, 1.0f); // FlatFace
	m_vertices[vert++].Pos = Vector3(0.0f, 0.0f, 0.0f);
	m_vertices[vert].Color = Color(boidRed, boidBlue, boidGreen, 1.0f);
	m_vertices[vert++].Pos = Vector3(2.5f, 1.25f, 0.0f);

	m_vertices[vert].Color = Color(boidRed, boidBlue, boidGreen, 1.0f);
	m_vertices[vert++].Pos = Vector3(2.5f, 0.0f, 5.0f);
	m_vertices[vert].Color = Color(boidRed, boidBlue, boidGreen, 1.0f); // Face2
	m_vertices[vert++].Pos = Vector3(2.5f, 1.25f, 0.0f);
	m_vertices[vert].Color = Color(boidRed, boidBlue, boidGreen, 1.0f);
	m_vertices[vert++].Pos = Vector3(0.0f, 0.0f, 0.0f);

	m_vertices[vert].Color = Color(boidRed, boidBlue, boidGreen, 1.0f);
	m_vertices[vert++].Pos = Vector3(2.5f, 1.25f, 0.0f);
	m_vertices[vert].Color = Color(boidRed, boidBlue, boidGreen, 1.0f); // Face3
	m_vertices[vert++].Pos = Vector3(2.5f, 0.0f, 5.0f);
	m_vertices[vert].Color = Color(boidRed, boidBlue, boidGreen, 1.0f);
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



void BoidPrey::activateBoid()
{
	isActive = true;
}


void BoidPrey::deactivateBoid()
{
	// Deactivate Boid
	isActive = false;

	// Set new Random Pos
	setRandPos();
}



void BoidPrey::run(std::vector<BoidPrey*>& _boids, GameData* _GD,
	int _boidGroup, BoidData* _boidData)
{
	maxSpeed = _boidData->boidMaxSpeed;
	if (maxSpeed < 0.1f)
	{
		maxSpeed = 0.1f;
	}

	neighbourDistance = _boidData->neighbourDist;
	desiredSeperation = _boidData->desiredSep;

	// If this boid, is in the current group to be updated, update behaviours...
	if (boidID >= _boidGroup && boidID <= (_boidGroup + 100))
	{
		flock(_boids, _GD, _boidData);
	}

	Tick(_GD);

	checkPosition();
}



void BoidPrey::flock(std::vector<BoidPrey*>& _boids, GameData* _GD, BoidData*& _boidData)
{
	Vector3 sep = m_separation->separate(this, _boidData, _boids);  // Seperation
	Vector3 ali = m_alignment->align(this, _boidData, _boids);      // Alignment
	Vector3 coh = m_cohesion->cohesion(this, _boidData, _boids);    // Cohesion
	Vector3 avoid = m_avoidance->avoid(this, _boidData, _GD);

	//groupColor(_boids);              // Function not currently working...

	// Arbitrarily weight these force
	sep *= _boidData->sepWeight;
	avoid *= _boidData->runWeight;
	ali *= _boidData->aliWeight;
	coh *= _boidData->cohWeight;

	applyForce(sep);
	applyForce(ali);
	applyForce(coh);
	applyForce(avoid);
}



void BoidPrey::applyForce(Vector3& force)
{
	acceleration = (acceleration + force);
}



void BoidPrey::Tick(GameData* _GD)
{
	// update velocity
	velocity = (velocity + acceleration);

	velocity = XMVector3Normalize(velocity);

	velocity = XMVector3ClampLength(velocity, 0.0f, maxSpeed);

	m_pos = (m_pos + velocity);

	Matrix scaleMat = Matrix::CreateScale(m_scale);
	Matrix rotTransMat = Matrix::CreateWorld(m_pos, -velocity, Vector3::Up);

	m_worldMat = m_fudge * scaleMat * rotTransMat;

	// reset acceleration to 0 each cycle
	acceleration = Vector3::Zero;
}



Vector3 BoidPrey::getVelocity()
{
	return velocity;
}



void BoidPrey::setRandPos()
{
	// Set random starting positon, inside the Designated zone
	float posX = randomFloat(-zoneSize, zoneSize);
	float posY = randomFloat(-zoneSize, zoneSize);
	float posZ = randomFloat(-zoneSize, zoneSize);

	m_pos = Vector3(posX, posY, posZ);
}



float BoidPrey::randomFloat(float _min, float _max)
{
	float r = (float)rand() / (float)RAND_MAX;
	return _min + r * (_max - _min);
}



void BoidPrey::checkPosition()
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
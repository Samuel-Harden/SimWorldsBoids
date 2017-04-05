#include "WayPoint.h"

#include "GameData.h"
#include "BoidData.h"
#include "Behaviour.h"

#include "PositionCheck.h"

WayPoint::WayPoint()
{
	// Set random starting positon, inside the Designated zone
	setRandPos();
}



WayPoint::~WayPoint()
{

}



void WayPoint::init(ID3D11Device* GD)
{

	//calculate number of vertices and primatives
	int numVerts = 36;
	m_numPrims = numVerts / 3;
	m_vertices = new myVertex[numVerts];
	WORD* indices = new WORD[numVerts];

	//as using the standard VB shader set the tex-coords somewhere safe
	for (int i = 0; i < numVerts; i++)
	{
		indices[i] = i;
		m_vertices[i].texCoord = Vector2::One;
	}

	int vert = 0;

	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(0.0f, 0.0f, 0.0f);
	m_vertices[vert].Color = Color(red, green, blue, 1.0f); 
	m_vertices[vert++].Pos = Vector3(5.0f, 0.f, 0.0f);
	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(5.0f, 0.0f, 5.0f);
	
	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(0.0f, 0.0f, 0.0f);
	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(5.0f, 0.0f, 5.0f);
	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(0.0f, 0.0f, 5.0f);
	
	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(0.0f, 0.0f, 0.0f);
	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(0.0f, 0.0f, 5.0f);
	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(0.0f, 5.0f, 0.0f);
	
	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(0.0f, 5.0f, 5.0f);
	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(0.0f, 5.0f, 0.0f);
	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(0.0f, 0.0f, 5.0f);

	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(5.0f, 0.0f, 0.0f);
	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(0.0f, 0.0f, 0.0f);
	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(0.0f, 5.0f, 0.0f);

	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(0.0f, 5.0f, 0.0f);
	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(5.0f, 5.0f, 0.0f);
	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(5.0f, 0.0f, 0.0f);

	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(5.0f, 5.0f, 0.0f);
	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(0.0f, 5.0f, 0.0f);
	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(0.0f, 5.0f, 5.0f);

	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(0.0f, 5.0f, 5.0f);
	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(5.0f, 5.0f, 5.0f);
	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(5.0f, 5.0f, 0.0f);

	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(0.0f, 5.0f, 5.0f);
	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(0.0f, 0.0f, 5.0f);
	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(5.0f, 0.0f, 5.0f);

	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(5.0f, 5.0f, 5.0f);
	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(0.0f, 5.0f, 5.0f);
	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(5.0f, 0.0f, 5.0f);

	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(5.0f, 0.0f, 0.0f);
	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(5.0f, 5.0f, 0.0f);
	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(5.0f, 5.0f, 5.0f);

	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(5.0f, 0.0f, 5.0f);
	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(5.0f, 0.0f, 0.0f);
	m_vertices[vert].Color = Color(red, green, blue, 1.0f);
	m_vertices[vert++].Pos = Vector3(5.0f, 5.0f, 5.0f);
	
	//calculate the normals for the basic lighting in the base shader
	for (int i = 0; i < m_numPrims; i++)
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



void WayPoint::Tick(GameData* _GD, Vector3 _wayPoint)
{
	m_pos = _wayPoint;

	VBGO::Tick(_GD);
}



void WayPoint::setRandPos()
{
	// Set random starting positon, inside the Designated zone
	float posX = randomFloat(-zoneSize, zoneSize);
	float posY = randomFloat(-zoneSize, zoneSize);
	float posZ = randomFloat(-zoneSize, zoneSize);

	m_pos = Vector3(posX, posY, posZ);
}



float WayPoint::randomFloat(float _min, float _max)
{
	float r = (float)rand() / (float)RAND_MAX;
	return _min + r * (_max - _min);
}